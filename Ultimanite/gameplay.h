#pragma once

#include "framework.h"

namespace Game
{
	inline bool bReady = false;
	inline bool bDroppedLoadingScreen = false;

	static void SpawnPickupAtLocation(UObject* ItemDefinition, int Count, FVector Location)
	{
		auto FortPickupAthena = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortPickupAthena"), Location, {});

		auto EntryCount = reinterpret_cast<int*>(__int64(FortPickupAthena) + __int64(Offsets::PrimaryPickupItemEntryOffset) + __int64(Offsets::CountOffset));
		auto EntryItemDefinition = reinterpret_cast<UObject**>(__int64(FortPickupAthena) + __int64(Offsets::PrimaryPickupItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));

		*EntryCount = Count;
		*EntryItemDefinition = ItemDefinition;

		Pickup::OnRep_PrimaryPickupItemEntry(FortPickupAthena);
		Pickup::TossPickup(FortPickupAthena, Location, Globals::Pawn, 6, true);
	}

	static bool IsMatchingGuid(FGuid A, FGuid B)
	{
		return A.A == B.A && A.B == B.B && A.C == B.C && A.D == B.D;
	}

	static void EquipInventoryItem(FGuid Guid)
	{
		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(__int64(Globals::FortInventory) + __int64(Offsets::InventoryOffset) + __int64(Offsets::ItemInstancesOffset));

		for (int i = 0; i < ItemInstances->Num(); i++)
		{
			auto CurrentItemInstance = ItemInstances->operator[](i);

			// Does the GUID in the inventory match with the one we are trying to equip
			if (IsMatchingGuid(Player::GetGuid(CurrentItemInstance), Guid))
			{
				// if the GUIDs match, equip the weapon
				Player::EquipWeaponByDefinition(Globals::Pawn, Player::GetItemDefinition(CurrentItemInstance), Guid);
			}
		}
	}

	static void HandlePickup(void* Params)
	{
		struct ServerHandlePickupParams
		{
			UObject* Pickup;
			float InFlyTime;
			FVector InStartDirection;
			bool bPlayPickupSound;
		};

		auto CurrentParams = (ServerHandlePickupParams*)Params;

		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(__int64(Globals::FortInventory) + __int64(Offsets::InventoryOffset) + __int64(Offsets::ItemInstancesOffset));

		if (CurrentParams->Pickup != nullptr)
		{
			// get world item definition from item entry
			UObject** WorldItemDefinition = reinterpret_cast<UObject**>(__int64(CurrentParams->Pickup) + __int64(Offsets::PrimaryPickupItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));
			TArray<QuickbarSlot> QuickbarSlots = *reinterpret_cast<TArray<QuickbarSlot>*>(reinterpret_cast<uintptr_t>(Globals::Quickbar) + Offsets::PrimaryQuickbarOffset + Offsets::SlotsOffset);

			for (int i = 0; i < QuickbarSlots.Num(); i++)
			{
				if (QuickbarSlots[i].Items.Data == 0)
				{
					if (i >= 6)
					{
						// no space left in inventory, we should replace the current focused quickbar with this new pickup.
						int* CurrentFocusedSlot = reinterpret_cast<int*>(__int64(Globals::Quickbar) + __int64(Offsets::PrimaryQuickbarOffset) + __int64(Offsets::CurrentFocusedSlotOffset));

						// do not replace pickaxe
						if (*CurrentFocusedSlot == 0)
						{
							continue;
						}

						i = *CurrentFocusedSlot;

						FGuid CurrentFocusedGUID = QuickbarSlots[*CurrentFocusedSlot].Items[0];

						// loop through item entries and see which item matches the current focused slot GUID
						for (int j = 0; i < ItemInstances->Num(); j++)
						{
							auto ItemInstance = ItemInstances->operator[](j);

							auto ItemEntryDefinition = reinterpret_cast<UObject**>(__int64(ItemInstance) + __int64(Offsets::ItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));
							auto ItemEntryGuid = reinterpret_cast<FGuid*>(__int64(ItemInstance) + __int64(Offsets::ItemEntryOffset) + __int64(Offsets::ItemGuidOffset));

							if (IsMatchingGuid(CurrentFocusedGUID, *ItemEntryGuid))
							{
								// spawn the item we are replacing as a pickup
								SpawnPickupAtLocation(*ItemEntryDefinition, 1, AActor::GetLocation(Globals::Pawn));
							}
						}

						// empty current slot
						Player::EmptySlot(Globals::Quickbar, *CurrentFocusedSlot);
					}

					// give player item
					Inventory::AddItemToInventoryWithUpdate(*WorldItemDefinition, EFortQuickBars::Primary, i, 1);

					// destroy pickup in world
					AActor::Destroy(CurrentParams->Pickup);

					break;
				}
			}
		}
	}

	static void HandleInventoryDrop(void* Params)
	{
		struct ServerAttemptInventoryDropParams
		{
			FGuid ItemGuid;
			int Count;
		};

		auto PawnLocation = AActor::GetLocation(Globals::Pawn);

		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(__int64(Globals::FortInventory) + __int64(Offsets::InventoryOffset) + __int64(Offsets::ItemInstancesOffset));
		auto RequestedGuid = ((ServerAttemptInventoryDropParams*)Params)->ItemGuid;

		auto QuickbarSlots = *reinterpret_cast<TArray<QuickbarSlot>*>(__int64(Globals::Quickbar) + __int64(Offsets::PrimaryQuickbarOffset) + __int64(Offsets::SlotsOffset));

		for (int i = 0; i < QuickbarSlots.Num(); i++)
		{
			if (QuickbarSlots[i].Items.Data != nullptr)
			{
				if (IsMatchingGuid(QuickbarSlots[i].Items[0], RequestedGuid))
				{
					// remove item we are dropping from quickbars
					Player::EmptySlot(Globals::Quickbar, i);

					// update inventory due to quickbars being updated
					Inventory::UpdateInventory();
				}
			}
		}

		for (int i = 0; i < ItemInstances->Num(); i++)
		{
			auto CurrentItemInstance = ItemInstances->operator[](i);
			auto CurrentGuid = Player::GetGuid(CurrentItemInstance);

			if (IsMatchingGuid(CurrentGuid, RequestedGuid))
			{
				// we know this weapon is the one we want, fetch item definition from ItemEntry
				auto ItemDefinition = reinterpret_cast<UObject**>(__int64(CurrentItemInstance) + __int64(Offsets::ItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));

				if (ItemDefinition)
				{
					// spawn item we dropped as a pickup
					SpawnPickupAtLocation(*ItemDefinition, 1, PawnLocation);
				}
			}
		}
	}

	static void LoadMatch()
	{
		Globals::Controller = GetFirstPlayerController(GetWorld());
		Globals::GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		Globals::GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");
		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{ -122398, -103873.02, 3962.51 }, {});
		Globals::PlayerState = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::PlayerStateOffset);

		UObject* CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Globals::Controller, 0, 0, 0, 0, 0, 0, 0);

		*reinterpret_cast<UObject**>(__int64(Globals::Controller) + __int64(Offsets::CheatManagerOffset)) = CheatManager;

		UObject* FortEngine = FindObject(L"FortEngine /Engine/Transient.FortEngine_");

		UObject* GameViewport = *reinterpret_cast<UObject**>(__int64(FortEngine) + __int64(Offsets::GameViewportOffset));
		UObject** CurrentConsole = reinterpret_cast<UObject**>(__int64(GameViewport) + __int64(Offsets::ViewportConsoleOffset));
		UObject* NewConsole = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.Console"), GameViewport, 0, 0, 0, 0, 0, 0, 0);

		// asign console to game viewport
		*CurrentConsole = NewConsole;

		// required so inventory does not get filled up quickly
		*reinterpret_cast<int*>(__int64(Globals::Controller) + __int64(Offsets::OverriddenBackpackSizeOffset)) = 999;
		
		uint8_t* ControllerBitField = reinterpret_cast<uint8_t*>(__int64(Globals::Controller) + __int64(Offsets::bInfiniteAmmo));

		// bInfiniteAmmo
		ControllerBitField[0] = 1;

		Player::Possess(Globals::Controller, Globals::Pawn);

		FString CurrentVersion = RuntimeOptions::GetGameVersion();

		// some builds do not have GetGameVersion, we will assume that they use StartMatch
		if (CurrentVersion.ToWString() != L"Unknown")
		{
			// builds that require GamePhase
			if (wcsstr(CurrentVersion.ToWString(), L"v4") ||
				wcsstr(CurrentVersion.ToWString(), L"v6") ||
				wcsstr(CurrentVersion.ToWString(), L"v7"))
			{
				EAthenaGamePhase* CurrentGamePhase = reinterpret_cast<EAthenaGamePhase*>(__int64(Globals::GameState) + __int64(Offsets::GamePhaseOffset));
				*CurrentGamePhase = EAthenaGamePhase::Aircraft;

				GameState::OnRep_GamePhase(Globals::GameState, EAthenaGamePhase::None);

				if (wcsstr(CurrentVersion.ToWString(), L"v6"))
				{
					Player::ShowBuildingFoundation(FindObject(L"LF_Athena_POI_15x15_C /Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_FloatingIsland"), EDynamicFoundationType::Static);
					Player::ShowBuildingFoundation(FindObject(L"LF_Athena_POI_75x75_C /Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Lake1"), EDynamicFoundationType::Static);
				}
			}

			// builds that require StartMatch
			if (wcsstr(CurrentVersion.ToWString(), L"v3") ||
				wcsstr(CurrentVersion.ToWString(), L"v5") ||
				wcsstr(CurrentVersion.ToWString(), L"v8") ||
				wcsstr(CurrentVersion.ToWString(), L"v9") ||
				wcsstr(CurrentVersion.ToWString(), L"v10"))
			{
				GameMode::StartMatch(Globals::GameMode);
			}
		}
		else
		{
			GameMode::StartMatch(Globals::GameMode);
		}

		Player::ServerReadyToStartMatch(Globals::Controller);

		UObject* HeadCharacterPart = FindObject(L"CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
		UObject* BodyCharacterPart = FindObject(L"CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

		if (HeadCharacterPart && BodyCharacterPart)
		{
			Player::ServerChoosePart(Globals::Pawn, EFortCustomPartType::Body, BodyCharacterPart);
			Player::ServerChoosePart(Globals::Pawn, EFortCustomPartType::Head, HeadCharacterPart);

			ProcessEvent(Globals::PlayerState, FindObject(L"Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterParts"), nullptr);
		}

	}

	namespace Hooks
	{
		void* ProcessEventDetour(UObject* Object, UObject* Function, void* Params)
		{
			auto ObjectName = Object->GetFullName();
			auto FunctionName = Function->GetFullName();

			// called when we load into a level
			if (wcsstr(FunctionName.c_str(), L"ReadyToStartMatch"))
			{
				if (!bReady)
				{
					bReady = true;

					// At this point, we are in the loading screen. Start loading into Athena_Terrain.
					LoadMatch();
				}
			}

			// called when an item is dropped from the inventory
			if (wcsstr(FunctionName.c_str(), L"ServerAttemptInventoryDrop"))
			{
				HandleInventoryDrop(Params);
			}

			// called when a player picks up a pickup
			if (wcsstr(FunctionName.c_str(), L"ServerHandlePickup"))
			{
				HandlePickup(Params);
			}

			// called when we select an item in inventory
			if (wcsstr(FunctionName.c_str(), L"ServerExecuteInventoryItem"))
			{
				EquipInventoryItem(*(FGuid*)Params);
			}

			if (wcsstr(FunctionName.c_str(), L"ServerAttemptExitVehicle"))
			{
				UObject* Vehicle = Player::GetVehicle();

				*reinterpret_cast<ENetRole*>(__int64(Globals::Pawn) + __int64(Offsets::RoleOffset)) = ENetRole::ROLE_Authority;
				*reinterpret_cast<ENetRole*>(__int64(Vehicle) + __int64(Offsets::RoleOffset)) = ENetRole::ROLE_Authority;
			}

			if (Object == Globals::Pawn && wcsstr(FunctionName.c_str(), L"Tick") && bDroppedLoadingScreen)
			{
				*reinterpret_cast<ENetRole*>(__int64(Globals::Pawn) + __int64(Offsets::RoleOffset)) = (Player::IsInVehicle() ? ENetRole::ROLE_AutonomousProxy : ENetRole::ROLE_Authority);

				UObject* Vehicle = Player::GetVehicle();

				if (Vehicle)
				{
					*reinterpret_cast<ENetRole*>(__int64(Vehicle) + __int64(Offsets::RoleOffset)) = ENetRole::ROLE_AutonomousProxy;
				}
			}

			if (wcsstr(FunctionName.c_str(), L"ServerLoadingScreenDropped"))
			{
				struct ToSlateBrush
				{
					FSlateBrush Brush;
				};

				reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapBackgroundBrushOffset))->ObjectResource = StaticLoadObjectEasy(FindObject(L"Class /Script/Engine.Texture2D"), L"/Game/Athena/HUD/MiniMap/MiniMapAthena.MiniMapAthena");

				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapSafeZoneBrushOffset))->Brush = {}; // MinimapCircleBrush
				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapCircleBrushOffset))->Brush = {}; // MinimapCircleBrush
				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapNextCircleBrushOffset))->Brush = {}; // MinimapCircleBrush
				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::FullMapCircleBrushOffset))->Brush = {}; // MinimapCircleBrush
				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::FullMapNextCircleBrushOffset))->Brush = {}; // MinimapCircleBrush
				reinterpret_cast<ToSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapSafeZoneBrushOffset))->Brush = {}; // MinimapCircleBrush

				// setup duktape
				UScript::SetupBindings();

				TArray<UObject*> FortHLODSMActors = GameplayStatics::GetAllActorsOfClass(FindObject(L"Class /Script/FortniteGame.FortHLODSMActor"));

				// destroy all FortHLODSMactor instances to remove HLODs
				for (int i = 0; i < FortHLODSMActors.Num(); i++)
				{
					AActor::Destroy(FortHLODSMActors[i]);
				}

				auto NetDebugUI = FindObject(L"NetDebugUI_C /Engine/Transient.FortEngine_0.FortGameInstance_0.AthenaHUD_C_0.WidgetTree_0.NetDebugContainer.WidgetTree_0.NetDebugUI");

				if (NetDebugUI)
				{
					// hide net debug UI in-game
					Widget::RemoveFromViewport(NetDebugUI);
				}

				// enable main menu in-game
				auto bHasServerFinishedLoading = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bHasServerFinishedLoadingOffset);
				*bHasServerFinishedLoading = true;

				Player::ServerSetClientHasFinishedLoading(Globals::Controller);
				
				// used to show username in top left
				PlayerState::OnRep_SquadId();

				Globals::FortInventory = reinterpret_cast<InventoryPointer*>(__int64(Globals::Controller) + __int64(Offsets::WorldInventoryOffset))->Inventory;
				Globals::Quickbar = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortQuickBars"), FVector{ -122398, -103873.02, 3962.51 }, {});

				// setup quickbar pointer
				reinterpret_cast<QuickBarPointer*>(__int64(Globals::Controller) + __int64(Offsets::QuickBarOffset))->QuickBar = Globals::Quickbar;

				// set owner of quickbar to current controller
				Player::SetOwner(Globals::Quickbar, Globals::Controller);

				// give gameplay abilities
				UObject** AbilitySystemComponent = reinterpret_cast<UObject**>(__int64(Globals::Pawn) + __int64(Offsets::AbilitySystemComponentOffset));

				if (AbilitySystemComponent)
				{
					Player::GrantGameplayAbility(FindObject(L"Class /Script/FortniteGame.FortGameplayAbility_Sprint"));
					Player::GrantGameplayAbility(FindObject(L"Class /Script/FortniteGame.FortGameplayAbility_Jump"));
					Player::GrantGameplayAbility(FindObject(L"BlueprintGeneratedClass /Game/Abilities/Player/Generic/Traits/DefaultPlayer/GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C"));
					Player::GrantGameplayAbility(FindObject(L"BlueprintGeneratedClass /Game/Abilities/Player/Generic/Traits/DefaultPlayer/GA_DefaultPlayer_InteractUse.GA_DefaultPlayer_InteractUse_C"));
					Player::GrantGameplayAbility(FindObject(L"BlueprintGeneratedClass /Game/Athena/DrivableVehicles/GA_AthenaEnterVehicle.GA_AthenaEnterVehicle_C"));
					Player::GrantGameplayAbility(FindObject(L"BlueprintGeneratedClass /Game/Athena/DrivableVehicles/GA_AthenaExitVehicle.GA_AthenaExitVehicle_C"));
					Player::GrantGameplayAbility(FindObject(L"BlueprintGeneratedClass /Game/Athena/DrivableVehicles/GA_AthenaInVehicle.GA_AthenaInVehicle_C"));
				}

				// give default items
				Inventory::AddItemToInventoryWithUpdate(FindObject(L"FortWeaponMeleeItemDefinition /Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"), EFortQuickBars::Primary, 0, 1);
				Inventory::AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall"), EFortQuickBars::Secondary, 0, 1);
				Inventory::AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor"), EFortQuickBars::Secondary, 1, 1);
				Inventory::AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W"), EFortQuickBars::Secondary, 2, 1);
				Inventory::AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS"), EFortQuickBars::Secondary, 3, 1);

				bDroppedLoadingScreen = true;
			}

			return ProcessEvent(Object, Function, Params);
		}
	}

	void Setup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(void*&)ProcessEvent, Hooks::ProcessEventDetour);

		DetourTransactionCommit();

		SetupOffsets();

		auto PlayerController = GetFirstPlayerController(GetWorld());

		if (PlayerController)
		{
			static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");
			PlayerController->Call(SwitchLevel, FString(L"Athena_Terrain?Game=/Game/Athena/Athena_GameMode.Athena_GameMode_C"));
		}
	}
}
