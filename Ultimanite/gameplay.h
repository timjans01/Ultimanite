#pragma once
#include "framework.h"

namespace Game
{
	inline bool bReady = false;
	inline bool bJumpedFromAircraft = false;

	static void JumpFromAircraft()
	{
		//Needs a fix!
		auto bus = GameState::GetAircraft(Globals::GameState);

		auto loc = AActor::GetLocation(bus);

		if (Globals::Pawn->isValid())
		{
			AActor::Destroy(Globals::Pawn);
		}

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), loc);
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
		ProcessEvent(Globals::Controller, FindObject(L"Function /Script/FortniteGame.FortPlayerController.HandleWorldInventoryLocalUpdate"), nullptr);
		ProcessEvent(Globals::FortInventory, FindObject(L"Function /Script/FortniteGame.FortInventory.HandleInventoryLocalUpdate"), nullptr);
		ProcessEvent(Globals::Controller, FindObject(L"Function /Script/FortniteGame.FortPlayerController.OnRep_QuickBar"), nullptr);
		ProcessEvent(Globals::Quickbar, FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_SecondaryQuickBar"), nullptr);
		ProcessEvent(Globals::Quickbar, FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_PrimaryQuickBar"), nullptr);
	}

	static void AddItemToInventory(UObject* FortItem, EFortQuickBars QuickbarIndex, int Slot)
	{
		struct ItemEntrySize
		{
			unsigned char Unk00[0xD0]; // Padding. TODO: DO DYNAMICALLY OR EVERYTHING WILL BREAK
		};

		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(Globals::FortInventory) + 0x328 + Offsets::ItemInstancesOffset);
		auto ItemEntries = reinterpret_cast<TArray<ItemEntrySize>*>(reinterpret_cast<uintptr_t>(Globals::FortInventory) + 0x328 + Offsets::ItemEntriesOffset);

		auto ItemEntry = reinterpret_cast<ItemEntrySize*>(reinterpret_cast<uintptr_t>(FortItem) + Offsets::ItemEntryOffset);

		ItemInstances->Add(FortItem); // First Offset: AFortInventory->Inventory // Second Offset: FFortItemList.ItemInstances
		ItemEntries->Add(*ItemEntry);

		Player::ServerAddItemInternal(Globals::Quickbar, Player::GetItemGuid(FortItem), QuickbarIndex, Slot);
	}


	static void LoadMatch()
	{
		Globals::Controller = GetFirstPlayerController(GetWorld());
		Globals::GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		Globals::GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");

		Globals::CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Globals::Controller, 0, 0, 0, 0, 0, 0, 0);

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{ 35000, 40562.594, 1300.150 });

		Player::Possess(Globals::Controller, Globals::Pawn);

		auto CurrentMovementStyle = (byte*)(uintptr_t(Globals::Pawn) + 0x7c4);

		*CurrentMovementStyle = 3;

		struct AthenaGameState
		{
			unsigned char Unk00[0x1cb0];
			EAthenaGamePhase GamePhase;
		};

		((AthenaGameState*)Globals::GameState)->GamePhase = EAthenaGamePhase::Warmup;

		GameState::OnRep_GamePhase(Globals::GameState, EAthenaGamePhase::None);

		Player::ServerReadyToStartMatch(Globals::Controller);



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
		Globals::Quickbar = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortQuickBars"), FVector{ 29481.783, 40562.594, 1237.150 });

		if (Globals::FortInventory && Globals::Quickbar)
		{
			reinterpret_cast<QuickBarPointer*>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::QuickBarOffset)->QuickBar = Globals::Quickbar;

			Player::SetOwner(Globals::FortInventory, Globals::Controller);
			Player::SetOwner(Globals::Quickbar, Globals::Controller);

			UObject* Pickaxe = CreateItem(FindObject(L"FortWeaponMeleeItemDefinition /Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"), 1);
			UObject* FirstWeapon = CreateItem(FindObject(L"FortWeaponRangedItemDefinition /Game/Items/Weapons/Ranged/Assault/SemiAuto_High/WID_Assault_SemiAuto_VR_Ore_T06.WID_Assault_SemiAuto_VR_Ore_T06"), 1);

			AddItemToInventory(Pickaxe, EFortQuickBars::Primary, 0);
			AddItemToInventory(FirstWeapon, EFortQuickBars::Primary, 1);

			UpdateInventory();
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

			return ProcessEvent(Object, Function, Params);
		}

		void TickPlayerInputHook(UObject* APlayerController, const float DeltaSeconds, const bool bGamePaused) 
		{
			TickPlayerInput(APlayerController, DeltaSeconds, bGamePaused);

			auto pawn = Controller::GetPawn(APlayerController);

			static bool bHasJumped;

			if (GetAsyncKeyState(VK_SPACE))
			{
				if (!bHasJumped)
				{
					bHasJumped = !bHasJumped;
					Player::Jump(Globals::Pawn);
				}
			}
			else 
			{
				bHasJumped = false;
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

		// Setup offsets
		Offsets::ItemInstancesOffset = FindOffset(L"ArrayProperty /Script/FortniteGame.FortItemList.ItemInstances");
		Offsets::ItemEntriesOffset = FindOffset(L"ArrayProperty /Script/FortniteGame.FortItemList.ReplicatedEntries");
		Offsets::ItemEntryOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortWorldItem.ItemEntry");
		Offsets::WorldInventoryOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerController.WorldInventory");
		Offsets::QuickBarOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerController.QuickBars");
		Offsets::GamePhaseOffset = FindOffset(L"EnumProperty /Script/FortniteGame.FortGameStateAthena.GamePhase");

		auto PlayerController = GetFirstPlayerController(GetWorld());

		if (PlayerController)
		{
			static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");

			PlayerController->Call(SwitchLevel, FString(L"Athena_Terrain?Game=/Game/Athena/Athena_GameMode.Athena_GameMode_C"));
		}
	}
}