#pragma once
#include "framework.h"

//#define PE_LOGGING

namespace Game
{
	inline bool bReady = false;
	inline bool bJumpedFromAircraft = false;
	inline bool bDroppedLoadingScreen = false;

	static void JumpFromAircraft()
	{
		auto bus = GameplayStatics::GetAllActorsOfClass(Globals::BattleBusClass)[0];

		auto loc = AActor::GetLocation(bus);

		if (Globals::Pawn->isValid())
		{
			AActor::Destroy(Globals::Pawn);
		}

		Globals::Pawn = SpawnActorEasy(GetWorld(), Globals::PawnClass, loc, {}); //TODO: FIX ROTATION
		Player::Possess(Globals::Controller, Globals::Pawn);
	}

	static UObject* CreateItem(UObject* ItemDefinition, int Count)
	{
		UObject* TemporaryItemInstance = Player::CreateTemporaryItemInstanceBP(ItemDefinition, Count, 1);

		if (TemporaryItemInstance)
		{
			Player::SetOwningControllerForTemporaryItem(TemporaryItemInstance, Globals::Controller);
		}

		if (Count > 0)
		{
			struct ItemCount
			{
				char Bytes[0xC];
				int Count;
			};

			reinterpret_cast<ItemCount*>(reinterpret_cast<uintptr_t>(TemporaryItemInstance) + Offsets::ItemEntryOffset)->Count = Count;
		}

		return TemporaryItemInstance;
	}

	static void UpdateInventory()
	{
		static auto HandleWorldInventoryLocalUpdate = FindObject(L"Function /Script/FortniteGame.FortPlayerController.HandleWorldInventoryLocalUpdate");
		static auto HandleInventoryLocalUpdate = FindObject(L"Function /Script/FortniteGame.FortInventory.HandleInventoryLocalUpdate");
		static auto OnRep_QuickBar = FindObject(L"Function /Script/FortniteGame.FortPlayerController.OnRep_QuickBar");
		static auto OnRep_SecondaryQuickBar = FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_SecondaryQuickBar");
		static auto OnRep_PrimaryQuickBar = FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_PrimaryQuickBar");

		ProcessEvent(Globals::Controller, HandleWorldInventoryLocalUpdate, nullptr);
		ProcessEvent(Globals::FortInventory, HandleInventoryLocalUpdate, nullptr);
		ProcessEvent(Globals::Controller, OnRep_QuickBar, nullptr);
		ProcessEvent(Globals::Quickbar, OnRep_SecondaryQuickBar, nullptr);
		ProcessEvent(Globals::Quickbar, OnRep_PrimaryQuickBar, nullptr);
	}

	static void AddItemToInventory(UObject* FortItem, EFortQuickBars QuickbarIndex, int Slot)
	{
		struct ItemEntrySize
		{
			unsigned char Unk00[0xD0]; // Padding. TODO: DO DYNAMICALLY OR EVERYTHING WILL BREAK
		};

		Globals::ItemInstances = reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(Globals::FortInventory) + 0x328 + Offsets::ItemInstancesOffset);
		auto ItemEntries = reinterpret_cast<TArray<ItemEntrySize>*>(reinterpret_cast<uintptr_t>(Globals::FortInventory) + 0x328 + Offsets::ItemEntriesOffset);

		auto ItemEntry = reinterpret_cast<ItemEntrySize*>(reinterpret_cast<uintptr_t>(FortItem) + Offsets::ItemEntryOffset);

		Globals::ItemInstances->Add(FortItem); // First Offset: AFortInventory->Inventory // Second Offset: FFortItemList.ItemInstances
		ItemEntries->Add(*ItemEntry);

		Player::ServerAddItemInternal(Globals::Quickbar, Player::GetItemGuid(FortItem), QuickbarIndex, Slot);
	}

	static void AddItemToInventoryWithUpdate(UObject* ItemDef, EFortQuickBars QuickbarIndex, int Slot, int Count)
	{
		UObject* ItemInstance = CreateItem(ItemDef, Count);

		AddItemToInventory(ItemInstance, QuickbarIndex, Slot);
		UpdateInventory();
	}

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

	static bool CompareGuids(FGuid A, FGuid B)
	{
		return A.A == B.A && A.B == B.B && A.C == B.C && A.D == B.D;
	}

	static void EquipInventoryItem(FGuid Guid)
	{
		int ItemsNumber = Globals::ItemInstances->Num();
		TArray<UObject*> ItemInstances = *Globals::ItemInstances;

		for (int i = 0; i < ItemsNumber; i++)
		{
			if (CompareGuids(Player::GetGuid(ItemInstances[i]), Guid))
			{
				Player::EquipWeaponDefinition(Globals::Pawn, Player::GetItemDefinition(ItemInstances[i]), Guid);
			}
		}
	}

	static void LoadMatch()
	{
		Globals::Controller = GetFirstPlayerController(GetWorld());
		Globals::GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		Globals::GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");

		Globals::CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Globals::Controller, 0, 0, 0, 0, 0, 0, 0);

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{-124398, -103873.02, 3962.51}, {});

		Globals::GamePlayStatics = FindObject(L"GameplayStatics /Script/Engine.Default__GameplayStatics");

		Player::Possess(Globals::Controller, Globals::Pawn);

		struct AthenaGameState
		{
			unsigned char Unk00[0x1cb0];
			EAthenaGamePhase GamePhase;
		};

		((AthenaGameState*)Globals::GameState)->GamePhase = EAthenaGamePhase::Aircraft;

		GameState::OnRep_GamePhase(Globals::GameState, EAthenaGamePhase::None);

		Player::ServerReadyToStartMatch(Globals::Controller);

		//i hate bit fields
		auto bInfiniteAmmoBitField = (uint8_t*)(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bInfiniteAmmo);

		bInfiniteAmmoBitField[0] = 1; //bInfiniteAmmo = true

		auto StrongMyHero = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::StrongMyHeroOffset);
		auto CharacterParts = *reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(StrongMyHero) + Offsets::CharacterPartsOffset);

		std::vector<UObject*> CharacterPartsVector;

		for (auto i = 0; i < CharacterParts.Num(); i++) CharacterPartsVector.push_back(CharacterParts[i]);

		for (auto i = 0; i < CharacterPartsVector.size(); i++)
		{
			auto AdditionalData = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(CharacterPartsVector[i]) + Offsets::AdditionalDataOffset);
			if (AdditionalData->IsA(FindObject(L"Class /Script/FortniteGame.CustomCharacterHeadData")))
			{
				Player::ServerChoosePart(Globals::Pawn, EFortCustomPartType::Head, CharacterPartsVector[i]);
			}
			else if (AdditionalData->IsA(FindObject(L"Class /Script/FortniteGame.CustomCharacterBodyPartData")))
			{
				Player::ServerChoosePart(Globals::Pawn, EFortCustomPartType::Body, CharacterPartsVector[i]);
			}
			else if (AdditionalData->IsA(FindObject(L"Class /Script/FortniteGame.CustomCharacterHatData")))
			{
				Player::ServerChoosePart(Globals::Pawn, EFortCustomPartType::Hat, CharacterPartsVector[i]);
			}
		}

		// TODO: Move to ConsoleCommandHook

		struct InventoryPointer
		{
			UObject* Inventory;
		};

		struct QuickBarPointer
		{
			UObject* QuickBar;
		};

		Globals::FortInventory = reinterpret_cast<InventoryPointer*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::WorldInventoryOffset)->Inventory;
		Globals::Quickbar = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortQuickBars"), FVector{29481.783, 40562.594, 1237.150}, {});

		if (Globals::FortInventory && Globals::Quickbar)
		{
			reinterpret_cast<QuickBarPointer*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::QuickBarOffset)->QuickBar = Globals::Quickbar;

			Player::SetOwner(Globals::FortInventory, Globals::Controller);
			Player::SetOwner(Globals::Quickbar, Globals::Controller);

			AddItemToInventoryWithUpdate(FindObject(L"FortWeaponMeleeItemDefinition /Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"), EFortQuickBars::Primary, 0, 1);

			AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall"), EFortQuickBars::Secondary, 0, 1);
			AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor"), EFortQuickBars::Secondary, 1, 1);
			AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W"), EFortQuickBars::Secondary, 2, 1);
			AddItemToInventoryWithUpdate(FindObject(L"FortBuildingItemDefinition /Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS"), EFortQuickBars::Secondary, 3, 1);
			AddItemToInventoryWithUpdate(FindObject(L"FortWeaponRangedItemDefinition /Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03"), EFortQuickBars::Primary, 1, 1);
			AddItemToInventoryWithUpdate(FindObject(L"FortWeaponRangedItemDefinition /Game/Athena/Items/Weapons/WID_RC_Rocket_Athena_SR_T03"), EFortQuickBars::Primary, 2, 1);
		}

		SpawnPickupAtLocation(FindObject(L"FortWeaponRangedItemDefinition /Game/Items/Weapons/Ranged/Shotgun/VacuumTube/WID_Shotgun_VacuumTube_VR_Ore_T05.WID_Shotgun_VacuumTube_VR_Ore_T05"), 1, FVector{35000, 40562.594, 1300.150});
	}

	static void HandleInventoryDrop(void* Params)
	{
		struct ServerAttemptInventoryDropParams
		{
			FGuid ItemGuid;
			int Count;
		};

		auto loc = AActor::GetLocation(Globals::Pawn);

		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(Globals::FortInventory) + 0x328 + Offsets::ItemInstancesOffset);
		auto RequestedGuid = ((ServerAttemptInventoryDropParams*)Params)->ItemGuid;

		struct QuickbarSlot
		{
			TArray<struct FGuid> Items;
			bool bEnabled;
			char Unk00[0x7];
		};

		auto QuickbarSlots = *reinterpret_cast<TArray<QuickbarSlot>*>(reinterpret_cast<uintptr_t>(Globals::Quickbar) + Offsets::PrimaryQuickbarOffset + Offsets::SlotsOffset);

		for (int j = 0; j < QuickbarSlots.Num(); j++)
		{
			if (QuickbarSlots[j].Items.Data != NULL)
			{
				if (CompareGuids(QuickbarSlots[j].Items[0], RequestedGuid))
				{
					Player::EmptySlot(Globals::Quickbar, j);
					UpdateInventory();
				}
			}
		}

		for (int i = 0; i < ItemInstances->Num(); i++)
		{
			auto CurrentItemInstance = ItemInstances->operator[](i);
			auto CurrentItemGuid = Player::GetGuid(CurrentItemInstance);

			if (RequestedGuid.A == CurrentItemGuid.A && RequestedGuid.B == CurrentItemGuid.B && RequestedGuid.C == CurrentItemGuid.C && RequestedGuid.D == CurrentItemGuid.D)
			{
				// we know this weapon is the one we want, fetch item definition from ItemEntry
				auto ItemDefinition = reinterpret_cast<UObject**>(__int64(CurrentItemInstance) + __int64(Offsets::ItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));

				if (ItemDefinition)
				{
					SpawnPickupAtLocation(*ItemDefinition, 1, loc);
				}
			}
		}
	}

	static void HandleGuidedMissle(UObject* Object)
	{
		if (bDroppedLoadingScreen)
		{
			if (wcsstr(Object->GetName().c_str(), L"B_RCRocket_Launcher_Athena_C_0"))
			{
				auto loc = AActor::GetLocation(Globals::Pawn);
				loc.X += 200;

				auto rocket = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/Items/Weapons/Abilities/RCRocket/B_PrjPawn_Athena_RCRocket.B_PrjPawn_Athena_RCRocket_C"), loc, {});

				Player::SetupRemoteControlPawn(rocket);
			}
		}
	}

	namespace Hooks
	{
		void* ProcessEventDetour(UObject* Object, UObject* Function, void* Params)
		{
			auto ObjectName = Object->GetFullName();
			auto FunctionName = Function->GetFullName();

			if (wcsstr(FunctionName.c_str(), L"ReadyToStartMatch"))
			{
				if (!bReady)
				{
					bReady = true;

					// At this point, we are in the loading screen. Start loading into Athena_Terrain.
					LoadMatch();
				}
			}

			//Jumping from the aircraft.
			if (wcsstr(FunctionName.c_str(), L"ServerAttemptAircraftJump") || wcsstr(FunctionName.c_str(), L"OnAircraftExitedDropZone"))
			{
				if (!bJumpedFromAircraft)
				{
					bJumpedFromAircraft = true;
					JumpFromAircraft();
				}
			}

			if (wcsstr(FunctionName.c_str(), L"ServerAttemptInventoryDrop"))
			{
				HandleInventoryDrop(Params);
			}

			if (wcsstr(FunctionName.c_str(), L"ServerLoadingScreenDropped"))
			{
				FSlateBrush EmptyBrush = Kismet::NoResourceBrush();

				reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapBackgroundBrushOffset))->ObjectResource = StaticLoadObjectEasy(FindObject(L"Class /Script/Engine.Texture2D"), L"/Game/Athena/HUD/MiniMap/MiniMapAthena.MiniMapAthena");

				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapSafeZoneBrushOffset)) = EmptyBrush; // MinimapCircleBrush
				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapCircleBrushOffset)) = EmptyBrush; // MinimapCircleBrush
				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapNextCircleBrushOffset)) = EmptyBrush; // MinimapCircleBrush
				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::FullMapCircleBrushOffset)) = EmptyBrush; // MinimapCircleBrush
				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::FullMapNextCircleBrushOffset)) = EmptyBrush; // MinimapCircleBrush
				*reinterpret_cast<FSlateBrush*>(__int64(Globals::GameState) + __int64(Offsets::MinimapSafeZoneBrushOffset)) = EmptyBrush; // MinimapCircleBrush

				//Spawning the player on the start island. (COMMENTED OUT UNTIL THE RELEASE)
				//Globals::Pawn->Call(FindObject(L"Function /Script/Engine.Actor.K2_TeleportTo"), FVector{ -124398, -103873.02, 3962.51 });

				auto LODS = GameplayStatics::GetAllActorsOfClass(FindObject(L"Class /Script/FortniteGame.FortHLODSMActor"));
				for (int i = 0; i < LODS.Num(); i++)
				{
					AActor::Destroy(LODS[i]);
				}

				PlayerState::OnRep_SquadId();
				auto NetDebugUi = FindObject(L"NetDebugUI_C /Engine/Transient.FortEngine_0.FortGameInstance_0.AthenaHUD_C_0.WidgetTree_0.NetDebugContainer.WidgetTree_0.NetDebugUI");
				Widget::RemoveFromViewport(NetDebugUi);

				auto bReadyToStartMatch = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bReadyToStartMatchOffset);
				*bReadyToStartMatch = true;
				auto bClientPawnIsLoaded = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bClientPawnIsLoadedOffset);
				*bClientPawnIsLoaded = true;
				auto bHasClientFinishedLoading = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bHasClientFinishedLoadingOffset);
				*bHasClientFinishedLoading = true;
				auto bHasServerFinishedLoading = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bHasServerFinishedLoadingOffset);
				*bHasServerFinishedLoading = true;

				Player::ServerSetClientHasFinishedLoading(Globals::Controller);

				bDroppedLoadingScreen = true;
			}

			if (wcsstr(FunctionName.c_str(), L"ServerExecuteInventoryItem"))
			{
				EquipInventoryItem(*(FGuid*)Params);
			}

			if (wcsstr(FunctionName.c_str(), L"OnPlayWeaponFireFX"))
			{
				if (GetAsyncKeyState(VK_LBUTTON)) HandleGuidedMissle(Object);
			}

			#ifdef PE_LOGGING
			//LMFAO
			if (FunctionName != L"EvaluateGraphExposedInputs" && FunctionName != L"ReceiveTick" && FunctionName != L"Tick" && FunctionName != L"OnSubmixEnvelope" && FunctionName != L"OnSubmixSpectralAnalysis" && FunctionName != L"OnMouse" && FunctionName != L"GetSubtitleVisibility" && FunctionName != L"Pulse" && FunctionName != L"BlueprintUpdateAnimation" && FunctionName != L"BlueprintPostEvaluateAnimation" && FunctionName != L"BlueprintModifyCamera" && FunctionName != L"BlueprintModifyPostProcess" && FunctionName != L"Loop Animation Curve" && FunctionName != L"GetValue" && FunctionName != L"OnSignificantTick" && FunctionName != L"ReceiveDrawHUD" && FunctionName != L"Chime Visual" && FunctionName != L"UpdateTime" && FunctionName != L"GetMutatorByClass" && FunctionName != L"OnUpdateDirectionalLightForTimeOfDay" && FunctionName != L"UpdatePreviousPositionAndVelocity" && FunctionName != L"IsCachedIsProjectileWeapon" && FunctionName != L"LockOn" && FunctionName != L"GetAbilityTargetingLevel" && FunctionName != L"ReadyToEndMatch")
			{
				printf("[Object]: %ls [Function]: %ls\n", ObjectName.c_str(), FunctionName.c_str());
			}
			#endif

		out: return ProcessEvent(Object, Function, Params);
		}

		void TickPlayerInputHook(UObject* APlayerController, const float DeltaSeconds, const bool bGamePaused)
		{
			TickPlayerInput(APlayerController, DeltaSeconds, bGamePaused);

			//auto pawn = Controller::GetPawn(APlayerController);

			if (bDroppedLoadingScreen)
			{
				auto CurrentMovementStyle = reinterpret_cast<byte*>(reinterpret_cast<uintptr_t>(Globals::Pawn) + Offsets::MovementStyleOffset);
				auto bWantsToSprint = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::bWantsToSprintOffset);

				if (*bWantsToSprint && !GetAsyncKeyState(VK_LBUTTON) && !GetAsyncKeyState(VK_RBUTTON))
				{
					*CurrentMovementStyle = 3;
				}
				else
				{
					*CurrentMovementStyle = 0;
				}

				auto bInAircraft = reinterpret_cast<bool*>(__int64(Globals::GameState) + __int64(Offsets::bInAircraftOffset));

				static bool bHasJumped;

				if (!*bInAircraft && GetAsyncKeyState(VK_SPACE))
				{
					if (!bHasJumped)
					{
						bHasJumped = !bHasJumped;
						if (Player::CanJump(Globals::Pawn))
						{
							Player::Jump(Globals::Pawn);
						}
					}
				}
				else
				{
					bHasJumped = false;
				}

				if (GetAsyncKeyState(VK_F7))
				{
					Sleep(1000);
					//DEBUG CODE
				}
			}
		}
	}


	void Setup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(void*&)ProcessEvent, Hooks::ProcessEventDetour);
		DetourAttach(&(void*&)TickPlayerInput, Hooks::TickPlayerInputHook);

		DetourTransactionCommit();

		SetupOffsets();

		Globals::PawnClass = FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
		Globals::BattleBusClass = FindObject(L"Class /Script/FortniteGame.FortAthenaAircraft");

		auto PlayerController = GetFirstPlayerController(GetWorld());

		if (PlayerController)
		{
			static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");

			PlayerController->Call(SwitchLevel, FString(L"Athena_Terrain?Game=/Game/Athena/Athena_GameMode.Athena_GameMode_C"));
		}
	}
}
