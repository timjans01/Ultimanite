#pragma once
#include "framework.h"

namespace Globals
{
	inline UObject* Controller;
	inline UObject* GameState;
	inline UObject* GameMode;
	inline UObject* CheatManager;
	inline UObject* Pawn;
	inline UObject* PawnClass;
	inline UObject* BattleBusClass;
	inline UObject* PlayerState;
	inline UObject* FortInventory;
	inline UObject* Quickbar;
	inline TArray<UObject*>* ItemInstances;
	inline UObject* GamePlayStatics;
	inline UObject* World;
}

namespace Offsets
{
	DWORD ItemInstancesOffset;
	DWORD ItemEntriesOffset;
	DWORD ItemEntryOffset;
	DWORD WorldInventoryOffset;
	DWORD QuickBarOffset;
	DWORD GamePhaseOffset;
	DWORD StrongMyHeroOffset;
	DWORD HeroCharacterPartsOffset;
	DWORD AdditionalDataOffset;
	DWORD PlayerStateOffset;
	DWORD FortItemEntryOffset;
	DWORD PrimaryPickupItemEntryOffset;
	DWORD CountOffset;
	DWORD ItemDefinitionOffset;
	DWORD CharacterPartsOffset;
	DWORD MovementStyleOffset;
	DWORD bWantsToSprintOffset;
	DWORD bInAircraftOffset;
	DWORD bInfiniteAmmo;
	DWORD CurrentWeapon;
	DWORD SlotsOffset;
	DWORD PrimaryQuickbarOffset;
	DWORD MinimapCircleBrushOffset;
	DWORD MinimapSafeZoneBrushOffset;
	DWORD MinimapBackgroundBrushOffset;
	DWORD MinimapNextCircleBrushOffset;
	DWORD FullMapCircleBrushOffset;
	DWORD FullMapNextCircleBrushOffset;
	DWORD MinimapSafeZoneFinalPosBrushOffset;
	DWORD bReadyToStartMatchOffset;
	DWORD bClientPawnIsLoadedOffset;
	DWORD bHasClientFinishedLoadingOffset;
	DWORD bHasServerFinishedLoadingOffset;
	DWORD CurrentBuildableClassOffset;
	DWORD LastBuildLocationOffset;
	DWORD LastBuildRotationOffset;
	DWORD PreferredQuickbarSlotOffset;
	DWORD ItemGuidOffset;
	DWORD CurrentFocusedSlotOffset;
	DWORD OverriddenBackpackSizeOffset;
	DWORD GameViewportOffset;
	DWORD ViewportConsoleOffset;
}

static void SetupOffsets()
{
	Offsets::ItemInstancesOffset = FindOffset(L"ArrayProperty /Script/FortniteGame.FortItemList.ItemInstances");
	Offsets::ItemEntriesOffset = FindOffset(L"ArrayProperty /Script/FortniteGame.FortItemList.ReplicatedEntries");
	Offsets::ItemEntryOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortWorldItem.ItemEntry");
	Offsets::WorldInventoryOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerController.WorldInventory");
	Offsets::QuickBarOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerController.QuickBars");
	Offsets::GamePhaseOffset = FindOffset(L"EnumProperty /Script/FortniteGame.FortGameStateAthena.GamePhase");
	Offsets::StrongMyHeroOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerControllerAthena.StrongMyHero");
	Offsets::CharacterPartsOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPlayerState.CharacterParts");
	Offsets::AdditionalDataOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.CustomCharacterPart.AdditionalData");
	Offsets::PlayerStateOffset = FindOffset(L"ObjectProperty /Script/Engine.Controller.PlayerState");
	Offsets::FortItemEntryOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortWorldItem.ItemEntry");
	Offsets::PrimaryPickupItemEntryOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortPickup.PrimaryPickupItemEntry");
	Offsets::CountOffset = FindOffset(L"IntProperty /Script/FortniteGame.FortItemEntry.Count");
	Offsets::ItemDefinitionOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortItemEntry.ItemDefinition");
	Offsets::MovementStyleOffset = FindOffset(L"ByteProperty /Script/FortniteGame.FortPawn.CurrentMovementStyle");
	Offsets::bWantsToSprintOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bWantsToSprint");
	Offsets::bInfiniteAmmo = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bInfiniteAmmo");
	Offsets::CurrentWeapon = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPawn.CurrentWeapon");
	Offsets::bInAircraftOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerStateAthena.bInAircraft");
	Offsets::SlotsOffset = FindOffset(L"ArrayProperty /Script/FortniteGame.QuickBar.Slots");
	Offsets::PrimaryQuickbarOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortQuickBars.PrimaryQuickBar");
	Offsets::CurrentFocusedSlotOffset = FindOffset(L"IntProperty /Script/FortniteGame.QuickBar.CurrentFocusedSlot");
	Offsets::MinimapBackgroundBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.MinimapBackgroundBrush");
	Offsets::MinimapSafeZoneBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.MinimapSafeZoneBrush");
	Offsets::MinimapCircleBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.MinimapCircleBrush");
	Offsets::MinimapNextCircleBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.MinimapNextCircleBrush");
	Offsets::FullMapCircleBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.FullMapCircleBrush");
	Offsets::FullMapNextCircleBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.FullMapNextCircleBrush");
	Offsets::MinimapSafeZoneFinalPosBrushOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortGameStateAthena.MinimapSafeZoneFinalPosBrush");
	Offsets::bReadyToStartMatchOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bReadyToStartMatch");
	Offsets::bClientPawnIsLoadedOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bClientPawnIsLoaded");
	Offsets::bHasClientFinishedLoadingOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bHasClientFinishedLoading");
	Offsets::bHasServerFinishedLoadingOffset = FindOffset(L"BoolProperty /Script/FortniteGame.FortPlayerController.bHasServerFinishedLoading");
	Offsets::CurrentBuildableClassOffset = FindOffset(L"ClassProperty /Script/FortniteGame.FortPlayerController.CurrentBuildableClass");
	Offsets::LastBuildLocationOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortPlayerController.LastBuildPreviewGridSnapLoc");
	Offsets::LastBuildRotationOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortPlayerController.LastBuildPreviewGridSnapRot");
	Offsets::PreferredQuickbarSlotOffset = FindOffset(L"IntProperty /Script/FortniteGame.FortWorldItemDefinition.PreferredQuickbarSlot");
	Offsets::ItemGuidOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortItemEntry.ItemGuid");
	Offsets::OverriddenBackpackSizeOffset = FindOffset(L"IntProperty /Script/FortniteGame.FortPlayerController.OverriddenBackpackSize");
	Offsets::GameViewportOffset = FindOffset(L"ObjectProperty /Script/Engine.Engine.GameViewport");
	Offsets::ViewportConsoleOffset = FindOffset(L"ObjectProperty /Script/Engine.GameViewportClient.ViewportConsole");
}

enum class EFortQuickBars : uint8_t
{
	Primary, Secondary, Max_None, EFortQuickBars_MAX,
};

struct FSlateBrush
{
	// lets really hope this doesn't change on any updates lmao
	unsigned char Unk00[0x48];
	UObject* ObjectResource; // 0x08
};

struct QuickbarSlot
{
	TArray<struct FGuid> Items;
	bool bEnabled;
	char Unk00[0x7];
};

namespace Kismet
{
	static auto FStringToFName(FString inString) -> FName
	{
		static auto lib = FindObject(L"KismetStringLibrary /Script/Engine.Default__KismetStringLibrary");
		static auto func = FindObject(L"Function /Script/Engine.KismetStringLibrary.Conv_StringToName");

		struct Params
		{
			FString s;
			FName ret;
		};

		Params params;
		params.s = inString;

		ProcessEvent(lib, func, &params);

		return params.ret;
	}

	static FSlateBrush NoResourceBrush()
	{
		static UObject* Default__WidgetBlueprintLibrary = FindObject(L"WidgetBlueprintLibrary /Script/UMG.Default__WidgetBlueprintLibrary");
		static UObject* NoResourceBrush = FindObject(L"Function /Script/UMG.WidgetBlueprintLibrary.NoResourceBrush");

		FSlateBrush ReturnValue;
		ProcessEvent(Default__WidgetBlueprintLibrary, NoResourceBrush, &ReturnValue);

		return ReturnValue;
	}
}

namespace Building
{
	static bool IsInBuildMode()
	{
		auto func = FindObject(L"Function /Script/FortniteGame.FortPlayerController.IsInBuildMode");

		bool poop;

		ProcessEvent(Globals::Controller, func, &poop);
		return poop;
	}

	static void InitializeBuildingActor(UObject* BuildingActor)
	{
		static UObject* InitializeKismetSpawnedBuildingActor = FindObject(L"Function /Script/FortniteGame.BuildingActor.InitializeKismetSpawnedBuildingActor");

		struct Params
		{
			UObject* BuildingOwner;
			UObject* SpawningController;
		};

		Params params;
		params.BuildingOwner = BuildingActor;
		params.SpawningController = Globals::Controller;

		ProcessEvent(BuildingActor, InitializeKismetSpawnedBuildingActor, &params);
	}
}

//TODO: Move to a class for multiplayer.
namespace Player
{
	static void Jump(UObject* Pawn)
	{
		static UObject* Jump = FindObject(L"Function /Script/Engine.Character.Jump");

		ProcessEvent(Pawn, Jump, nullptr);
	}

	static bool CanJump(UObject* Pawn)
	{
		static UObject* Jump = FindObject(L"Function /Script/Engine.Character.CanJump");

		bool ReturnValue;

		ProcessEvent(Pawn, Jump, &ReturnValue);

		return ReturnValue;
	}

	static void SwitchLevel(UObject* InController, FString URL)
	{
		static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");

		ProcessEvent(InController, SwitchLevel, &URL);
	}

	static void ServerReadyToStartMatch(UObject* InController)
	{
		static UObject* ServerReadyToStartMatch = FindObject(L"Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch");

		ProcessEvent(InController, ServerReadyToStartMatch, nullptr);
	}

	static void Possess(UObject* InController, UObject* InPawn)
	{
		static UObject* Possess = FindObject(L"Function /Script/Engine.Controller.Possess");

		ProcessEvent(InController, Possess, &InPawn);
	}

	static FGuid GetItemGuid(UObject* FortItem)
	{
		static UObject* GetItemGuid = FindObject(L"Function /Script/FortniteGame.FortItem.GetItemGuid");
		FGuid ReturnValue;

		ProcessEvent(FortItem, GetItemGuid, &ReturnValue);
		return ReturnValue;
	}

	static void ServerAddItemInternal(UObject* Quickbars, FGuid Item, EFortQuickBars Quickbar, int Slot)
	{
		static UObject* ServerAddItemInternal = FindObject(L"Function /Script/FortniteGame.FortQuickBars.ServerAddItemInternal");

		struct
		{
			FGuid Item;
			EFortQuickBars Quickbar;
			int Slot;
		} Params;

		Params.Item = Item;
		Params.Quickbar = Quickbar;
		Params.Slot = Slot;

		ProcessEvent(Quickbars, ServerAddItemInternal, &Params);
	}

	static UObject* CreateTemporaryItemInstanceBP(UObject* ItemDefinition, int Count, int Level)
	{
		static UObject* CreateTemporaryItemInstanceBP = FindObject(L"Function /Script/FortniteGame.FortItemDefinition.CreateTemporaryItemInstanceBP");

		struct
		{
			int Count;
			int Level;
			UObject* ReturnValue;
		} Params;

		Params.Count = Count;
		Params.Level = Level;

		ProcessEvent(ItemDefinition, CreateTemporaryItemInstanceBP, &Params);

		return Params.ReturnValue;
	}

	static auto SetupRemoteControlPawn(UObject* Rocket)
	{
		static auto func = FindObject(L"Function /Script/FortniteGame.FortRemoteControlledPawnAthena.SetupRemoteControlPawn");


		struct Params
		{
			UObject* Controller;
			UObject* Pawn;
			byte MovementMode;

			//ONLY FOR 4.1
			FFortGameplayEffectContainerSpec EffectContainerSpecOnKill;
		};

		Params params;
		params.Controller = Globals::Controller;
		params.Pawn = Globals::Pawn;
		params.MovementMode = 6;
		params.EffectContainerSpecOnKill = FFortGameplayEffectContainerSpec{};

		ProcessEvent(Rocket, func, &params);
	}

	static void SetOwningControllerForTemporaryItem(UObject* Item, UObject* Controller)
	{
		static UObject* SetOwningControllerForTemporaryItem = FindObject(L"Function /Script/FortniteGame.FortItem.SetOwningControllerForTemporaryItem");

		ProcessEvent(Item, SetOwningControllerForTemporaryItem, &Controller);
	}

	static void SetOwner(UObject* TargetActor, UObject* NewOwner)
	{
		static UObject* SetOwner = FindObject(L"Function /Script/Engine.Actor.SetOwner");

		ProcessEvent(TargetActor, SetOwner, &NewOwner);
	}

	static void ServerChoosePart(UObject* Target, TEnumAsByte<EFortCustomPartType> Type, UObject* ChosenCharacterPart)
	{
		static UObject* ServerChoosePart = FindObject(L"Function /Script/FortniteGame.FortPlayerPawn.ServerChoosePart");

		struct
		{
			TEnumAsByte<EFortCustomPartType> Type;
			UObject* ChosenCharacterPart;
		} Params;

		Params.Type = Type;
		Params.ChosenCharacterPart = ChosenCharacterPart;

		ProcessEvent(Target, ServerChoosePart, &Params);
	}

	static FGuid GetGuid(UObject* Target)
	{
		static UObject* GetItemGuid = FindObject(L"Function /Script/FortniteGame.FortItem.GetItemGuid");

		FGuid ReturnValue;

		ProcessEvent(Target, GetItemGuid, &ReturnValue);

		return ReturnValue;
	}

	static UObject* GetItemDefinition(UObject* Target)
	{
		static UObject* GetItemGuid = FindObject(L"Function /Script/FortniteGame.FortItem.GetItemDefinitionBP");

		UObject* ReturnValue;

		ProcessEvent(Target, GetItemGuid, &ReturnValue);

		return ReturnValue;
	}

	static UObject* EquipWeaponDefinition(UObject* Target, UObject* ItemDefinition, FGuid ItemGuid)
	{
		static UObject* GetItemGuid = FindObject(L"Function /Script/FortniteGame.FortPawn.EquipWeaponDefinition");

		struct
		{
			UObject* ItemDefinition;
			FGuid ItemGuid;
			UObject* ReturnValue;
		} params;

		params.ItemDefinition = ItemDefinition;
		params.ItemGuid = ItemGuid;

		ProcessEvent(Target, GetItemGuid, &params);

		return params.ReturnValue;
	}

	static void EmptySlot(UObject* Target, int Slot)
	{
		static UObject* EmptySlot = FindObject(L"Function /Script/FortniteGame.FortQuickBars.EmptySlot");

		struct
		{
			EFortQuickBars QuickbarIndex;
			int Slot;
		} params;

		params.QuickbarIndex = EFortQuickBars::Primary;
		params.Slot = Slot;

		ProcessEvent(Target, EmptySlot, &params);
	}

	static void ServerSetClientHasFinishedLoading(UObject* Target)
	{
		static UObject* ServerSetClientHasFinishedLoading = FindObject(L"Function /Script/FortniteGame.FortPlayerController.ServerSetClientHasFinishedLoading");

		bool HasFinishedLoading = true;

		ProcessEvent(Target, ServerSetClientHasFinishedLoading, &HasFinishedLoading);
	}
}

namespace PlayerState
{
	static void OnRep_SquadId()
	{
		static UObject* OnRep_SquadID = FindObject(L"Function /Script/FortniteGame.FortPlayerStateAthena.OnRep_SquadId");

		Globals::PlayerState = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::Controller) + Offsets::PlayerStateOffset);
		ProcessEvent(Globals::PlayerState, OnRep_SquadID, nullptr);
	};
}

namespace Pickup
{
	static void TossPickup(UObject* FortPickup, FVector FinalLocation, UObject* ItemOwner, int OverrideMaxStackCount, bool bToss)
	{
		static UObject* TossPickup = FindObject(L"Function /Script/FortniteGame.FortPickup.TossPickup");

		struct
		{
			FVector FinalLocation;
			UObject* ItemOwner;
			int OverrideMaxStackCount;
			bool bToss;
		} Params;

		Params.FinalLocation = FinalLocation;
		Params.ItemOwner = ItemOwner;
		Params.OverrideMaxStackCount = OverrideMaxStackCount;
		Params.bToss = bToss;

		ProcessEvent(FortPickup, TossPickup, &Params);
	}

	static void OnRep_PrimaryPickupItemEntry(UObject* FortPickup)
	{
		static UObject* OnRep_PrimaryPickupItemEntry = FindObject(L"Function /Script/FortniteGame.FortPickup.OnRep_PrimaryPickupItemEntry");

		ProcessEvent(FortPickup, OnRep_PrimaryPickupItemEntry, nullptr);
	}
}

namespace Controller
{
	static auto GetPawn(UObject* Controller)
	{
		static UObject* K2_GetPawn = FindObject(L"Function /Script/Engine.Controller.K2_GetPawn");

		struct Params
		{
			UObject* ret;
		};

		Params params;

		ProcessEvent(Controller, K2_GetPawn, &params);

		return params.ret;
	}

	static auto GetControlRotation(UObject* Target)
	{
		static UObject* GetControlRotation = FindObject(L"Function /Script/Engine.Controller.GetControlRotation");

		FRotator ReturnValue;

		ProcessEvent(Target, GetControlRotation, &ReturnValue);

		return ReturnValue;
	}
}

namespace GameState
{
	static UObject* GetAircraft(UObject* InGameState)
	{
		static UObject* GetAircraft = FindObject(L"Function /Script/FortniteGame.FortGameStateAthena.GetAircraft");

		struct Params
		{
			int i;
			UObject* bus;
		};

		Params params;

		params.i = 1;

		ProcessEvent(InGameState, GetAircraft, &params);
		return params.bus;
	}

	static void OnRep_GamePhase(UObject* InGameState, EAthenaGamePhase OldGamePhase)
	{
		static UObject* OnRep_GamePhase = FindObject(L"Function /Script/FortniteGame.FortGameStateAthena.OnRep_GamePhase");

		ProcessEvent(InGameState, OnRep_GamePhase, &OldGamePhase);
	}
}

namespace CheatManager
{
	static void Summon(UObject* InCheatManager, FString InClassName)
	{
		static UObject* Summon = FindObject(L"Function /Script/Engine.CheatManager.Summon");

		ProcessEvent(InCheatManager, Summon, &InClassName);
	}
}

namespace GameplayStatics
{
	static TArray<UObject*> GetAllActorsOfClass(UObject* Class)
	{
		struct Parameters
		{
			UObject* World;
			UObject* Class;
			TArray<UObject*> Return;
		};

		Parameters parameters;
		parameters.World = GetWorld();
		parameters.Class = Class;

		ProcessEvent(Globals::GamePlayStatics, FindObject(L"Function /Script/Engine.GameplayStatics.GetAllActorsOfClass"), &parameters);

		return parameters.Return;
	}
}

namespace AActor
{
	static FVector GetLocation(UObject* Target)
	{
		static auto K2_GetActorLocation = FindObject(L"Function /Script/Engine.Actor.K2_GetActorLocation");

		struct
		{
			FVector ret;
		} Params;

		ProcessEvent(Target, K2_GetActorLocation, &Params);

		return Params.ret;
	}

	static void Destroy(UObject* Target)
	{
		static auto K2_DestroyActor = FindObject(L"Function /Script/Engine.Actor.K2_DestroyActor");

		ProcessEvent(Target, K2_DestroyActor, nullptr);
	}
}

namespace Widget
{
	static void RemoveFromViewport(UObject* Target)
	{
		static UObject* RemoveFromViewport = FindObject(L"Function /Script/UMG.UserWidget.RemoveFromViewport");

		ProcessEvent(Target, RemoveFromViewport, nullptr);
	}
}
