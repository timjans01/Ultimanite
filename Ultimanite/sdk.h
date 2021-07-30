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
	inline duk_context* DukContext;
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
	DWORD InventoryOffset;
	DWORD CountOffset;
	DWORD ItemDefinitionOffset;
	DWORD CharacterPartsOffset;
	DWORD MovementStyleOffset;
	DWORD bWantsToSprintOffset;
	DWORD bInAircraftOffset;
	DWORD bInfiniteAmmo;
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
	DWORD CurrentPlaylistDataOffset;
	DWORD WeaponActorClassOffset;
	DWORD WeaponDataOffset;
	DWORD ItemEntryGuidOffset;
	DWORD AmmoCountOffset;
	DWORD CurrentWeaponOffset;
	DWORD AbilitySystemComponentOffset;
	DWORD DurationPolicyOffset;
	DWORD GrantedAbilitiesOffset;
	DWORD AbilityOffset;
	DWORD TextRenderOffset;
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
	Offsets::CurrentWeaponOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPawn.CurrentWeapon");
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
	Offsets::CurrentPlaylistDataOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortGameStateAthena.CurrentPlaylistData");
	Offsets::InventoryOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortInventory.Inventory");
	Offsets::WeaponActorClassOffset = FindOffset(L"SoftClassProperty /Script/FortniteGame.FortWeaponItemDefinition.WeaponActorClass");
	Offsets::WeaponDataOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortWeapon.WeaponData");
	Offsets::ItemEntryGuidOffset = FindOffset(L"StructProperty /Script/FortniteGame.FortWeapon.ItemEntryGuid");
	Offsets::AmmoCountOffset = FindOffset(L"IntProperty /Script/FortniteGame.FortWeapon.AmmoCount");
	Offsets::AbilitySystemComponentOffset = FindOffset(L"ObjectProperty /Script/FortniteGame.FortPawn.AbilitySystemComponent");
	Offsets::DurationPolicyOffset = FindOffset(L"EnumProperty /Script/GameplayAbilities.GameplayEffect.DurationPolicy");
	Offsets::GrantedAbilitiesOffset = FindOffset(L"ArrayProperty /Script/GameplayAbilities.GameplayEffect.GrantedAbilities");
	Offsets::AbilityOffset = FindOffset(L"ClassProperty /Script/GameplayAbilities.GameplayAbilitySpecDef.Ability");
	Offsets::TextRenderOffset = FindOffset(L"ObjectProperty /Script/Engine.TextRenderActor.TextRender");
}

enum class EFortQuickBars : uint8_t
{
	Primary,
	Secondary,
	Max_None,
	EFortQuickBars_MAX
};

enum class EGameplayEffectDurationType : uint8_t
{
	Instant,
	Infinite,
	HasDuration,
	EGameplayEffectDurationType_MAX
};

struct FSlateBrush
{
	unsigned char Unk00[0x48];
	UObject* ObjectResource; // 0x08
};

struct QuickbarSlot
{
	TArray<struct FGuid> Items;
	bool bEnabled;
	char Unk00[0x7];
};

struct InventoryPointer
{
	UObject* Inventory;
};

struct QuickBarPointer
{
	UObject* QuickBar;
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

	static auto FNameToFString(FName InName)
	{
		static auto lib = FindObject(L"KismetStringLibrary /Script/Engine.Default__KismetStringLibrary");
		static auto func = FindObject(L"Function /Script/Engine.KismetStringLibrary.Conv_NameToString");

		struct
		{
			FName In;
			FString ret;
		} Params;

		Params.In = InName;

		ProcessEvent(lib, func, &Params);

		return Params.ret;
	}

	static FSlateBrush NoResourceBrush()
	{
		static UObject* Default__WidgetBlueprintLibrary = FindObject(L"WidgetBlueprintLibrary /Script/UMG.Default__WidgetBlueprintLibrary");
		static UObject* NoResourceBrush = FindObject(L"Function /Script/UMG.WidgetBlueprintLibrary.NoResourceBrush");

		FSlateBrush ReturnValue;
		ProcessEvent(Default__WidgetBlueprintLibrary, NoResourceBrush, &ReturnValue);

		return ReturnValue;
	}

	static UObject* Conv_SoftClassReferenceToClass(TSoftObjectPtr<UObject*> SoftPtr)
	{
		static UObject* Default__KismetSystemLibrary = FindObject(L"KismetSystemLibrary /Script/Engine.Default__KismetSystemLibrary");
		static UObject* Conv_SoftClassReferenceToClass = FindObject(L"Function /Script/Engine.KismetSystemLibrary.Conv_SoftClassReferenceToClass");

		struct
		{
			TSoftObjectPtr<UObject*> SoftClassReference;
			UObject* Class;
		} Params;

		Params.SoftClassReference = SoftPtr;

		ProcessEvent(Default__KismetSystemLibrary, Conv_SoftClassReferenceToClass, &Params);

		return Params.Class;
	}
}

namespace Weapon
{
	static void OnRep_ReplicatedWeaponData(UObject* Weapon)
	{
		static UObject* OnRep_ReplicatedWeaponData = FindObject(L"Function /Script/FortniteGame.FortWeapon.OnRep_ReplicatedWeaponData");

		ProcessEvent(Weapon, OnRep_ReplicatedWeaponData, nullptr);
	}

	static void ClientGivenTo(UObject* Weapon, UObject* Pawn)
	{
		static UObject* ClientGivenTo = FindObject(L"Function /Script/FortniteGame.FortWeapon.ClientGivenTo");

		ProcessEvent(Weapon, ClientGivenTo, &Pawn);
	}

	static void OnRep_AmmoCount(UObject* Weapon)
	{
		static UObject* OnRep_AmmoCount = FindObject(L"Function /Script/FortniteGame.FortWeapon.OnRep_AmmoCount");
		int OldAmmoCount = 0;

		ProcessEvent(Weapon, OnRep_AmmoCount, &OldAmmoCount);
	}
}

namespace GameMode
{
	static void StartMatch(UObject* InGameMode)
	{
		static UObject* StartMatch = FindObject(L"Function /Script/Engine.GameMode.StartMatch");

		ProcessEvent(InGameMode, StartMatch, nullptr);
	}
}

namespace RuntimeOptions
{
	static FString GetGameVersion()
	{
		static UObject* Default__FortRuntimeOptions = FindObject(L"FortRuntimeOptions /Script/FortniteGame.Default__FortRuntimeOptions");
		static UObject* GetGameVersion = FindObject(L"Function /Script/FortniteGame.FortRuntimeOptions.GetGameVersion");
		
		if (GetGameVersion)
		{
			FString GameVersion;
			ProcessEvent(Default__FortRuntimeOptions, GetGameVersion, &GameVersion);

			return GameVersion;
		}
		else
		{
			return L"Unknown";
		}
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

	static void SetOwningControllerForTemporaryItem(UObject* Item, UObject* Controller)
	{
		static UObject* SetOwningControllerForTemporaryItem = FindObject(L"Function /Script/FortniteGame.FortItem.SetOwningControllerForTemporaryItem");

		ProcessEvent(Item, SetOwningControllerForTemporaryItem, &Controller);
	}

	static void BP_ApplyGameplayEffectToSelf(UObject* AbilitySystemComponent, UObject* GameplayEffectClass)
	{
		static UObject* BP_ApplyGameplayEffectToSelf = FindObject(L"Function /Script/GameplayAbilities.AbilitySystemComponent.BP_ApplyGameplayEffectToSelf");
	
		struct
		{
			UObject* GameplayEffectClass;
			float Level;
			FGameplayEffectContextHandle EffectContext;
			FActiveGameplayEffectHandle ret;
		} Params;

		Params.EffectContext = FGameplayEffectContextHandle();
		Params.GameplayEffectClass = GameplayEffectClass;
		Params.Level = 1.0;

		ProcessEvent(AbilitySystemComponent, BP_ApplyGameplayEffectToSelf, &Params);
	}

	static void GrantGameplayAbility(UObject* GameplayAbilityClass)
	{
		UObject** AbilitySystemComponent = reinterpret_cast<UObject**>(__int64(Globals::Pawn) + __int64(Offsets::AbilitySystemComponentOffset));
		UObject* DefaultGameplayEffect = FindObject(L"GE_Athena_PurpleStuff_C /Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff.Default__GE_Athena_PurpleStuff_C");

		TArray<struct FGameplayAbilitySpecDef>* GrantedAbilities = reinterpret_cast<TArray<struct FGameplayAbilitySpecDef>*>(__int64(DefaultGameplayEffect) + __int64(Offsets::GrantedAbilitiesOffset));

		// overwrite current gameplay ability with the one we want to activate
		GrantedAbilities->operator[](0).Ability = GameplayAbilityClass;

		// give this gameplay effect an infinite duration
		*reinterpret_cast<EGameplayEffectDurationType*>(__int64(DefaultGameplayEffect) + __int64(Offsets::DurationPolicyOffset)) = EGameplayEffectDurationType::Infinite;

		// apply modified gameplay effect to ability system component
		BP_ApplyGameplayEffectToSelf(*AbilitySystemComponent, FindObject(L"BlueprintGeneratedClass /Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff.GE_Athena_PurpleStuff_C"));
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

	static void ClientInternalEquipWeapon(UObject* Pawn, UObject* FortWeapon)
	{
		static UObject* ClientInternalEquipWeapon = FindObject(L"Function /Script/FortniteGame.FortPawn.ClientInternalEquipWeapon");

		ProcessEvent(Pawn, ClientInternalEquipWeapon, &FortWeapon);
	}

	static void EquipWeaponByDefinition(UObject* Pawn, UObject* WeaponDefinition, FGuid ItemGuid)
	{
		TSoftObjectPtr<UObject*>* SoftWeaponActorClass = reinterpret_cast<TSoftObjectPtr<UObject*>*>(__int64(WeaponDefinition) + __int64(Offsets::WeaponActorClassOffset)); // TODO: change 0x708
		UObject* WeaponActorClass = Kismet::Conv_SoftClassReferenceToClass(*SoftWeaponActorClass);

		if (WeaponActorClass)
		{
			UObject* CurrentWeaponActor = SpawnActorEasy(GetWorld(), WeaponActorClass, {}, {});

			// set owner of weapon actor
			Player::SetOwner(CurrentWeaponActor, Globals::Pawn);

			// set weapon definition and GUID
			*reinterpret_cast<UObject**>(__int64(CurrentWeaponActor) + __int64(Offsets::WeaponDataOffset)) = WeaponDefinition;
			*reinterpret_cast<FGuid*>(__int64(CurrentWeaponActor) + __int64(Offsets::ItemEntryGuidOffset)) = ItemGuid;

			// replicate weapon data and ammo, then give weapon to pawn
			Weapon::OnRep_ReplicatedWeaponData(CurrentWeaponActor);
			Weapon::ClientGivenTo(CurrentWeaponActor, Pawn);

			// equip weapon
			ClientInternalEquipWeapon(Pawn, CurrentWeaponActor);
		}
	}

	// deprecated, use EquipWeaponByDefinition
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

	static void SpawnPickupAtLocation(UObject* ItemDefinition, int Count, FVector Location)
	{
		auto FortPickupAthena = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortPickupAthena"), Location, {});

		auto EntryCount = reinterpret_cast<int*>(__int64(FortPickupAthena) + __int64(Offsets::PrimaryPickupItemEntryOffset) + __int64(Offsets::CountOffset));
		auto EntryItemDefinition = reinterpret_cast<UObject**>(__int64(FortPickupAthena) + __int64(Offsets::PrimaryPickupItemEntryOffset) + __int64(Offsets::ItemDefinitionOffset));

		*EntryCount = Count;
		*EntryItemDefinition = ItemDefinition;

		OnRep_PrimaryPickupItemEntry(FortPickupAthena);
		TossPickup(FortPickupAthena, Location, Globals::Pawn, 6, true);
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

	static void OnRep_CurrentPlaylistData(UObject* InGameState)
	{
		static UObject* OnRep_CurrentPlaylistData = FindObject(L"Function /Script/FortniteGame.FortGameStateAthena.OnRep_CurrentPlaylistData");

		ProcessEvent(InGameState, OnRep_CurrentPlaylistData, nullptr);
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

namespace TextActor
{
	static UObject* Spawn(FVector Location = {}, FRotator Rotation = {})
	{
		static auto c = FindObject(L"Class /Script/Engine.TextRenderActor");

		auto TextRenderComponent = *(UObject**)(reinterpret_cast<__int64>(SpawnActorEasy(Globals::World, c, Location, Rotation)) + Offsets::TextRenderOffset /* UTextRenderComponent */);

		return TextRenderComponent;
	}

	static void SetText(UObject* Target, FString Text)
	{
		const auto SetText = FindObject(L"Function /Script/Engine.TextRenderComponent.SetText");

		ProcessEvent(Target, SetText, &Text);
	}

	static void SetWorldSize(UObject* Target, float value)
	{
		static auto SetWorldSize = FindObject(L"Function /Script/Engine.TextRenderComponent.SetWorldSize");

		ProcessEvent(Target, SetWorldSize, &value);
	}

	static void SetXScale(UObject* Target, float value)
	{
		static auto SetXScale = FindObject(L"Function /Script/Engine.TextRenderComponent.SetXScale");

		ProcessEvent(Target, SetXScale, &value);
	}

	static void SetYScale(UObject* Target, float value)
	{
		static auto SetYScale = FindObject(L"Function /Script/Engine.TextRenderComponent.SetYScale");

		ProcessEvent(Target, SetYScale, &value);
	}
}

namespace GameplayStatics
{
	static TArray<UObject*> GetAllActorsOfClass(UObject* Class)
	{
		static UObject* Default__GameplayStatics = FindObject(L"GameplayStatics /Script/Engine.Default__GameplayStatics");
		static UObject* GetAllActorsOfClass = FindObject(L"Function /Script/Engine.GameplayStatics.GetAllActorsOfClass");

		struct
		{
			UObject* World;
			UObject* Class;
			TArray<UObject*> ReturnValue;
		} Params;

		Params.World = GetWorld();
		Params.Class = Class;

		ProcessEvent(Default__GameplayStatics, GetAllActorsOfClass, &Params);

		return Params.ReturnValue;
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

	static FRotator GetRotation(UObject* Target)
	{
		static auto K2_GetActorRotation = FindObject(L"Function /Script/Engine.Actor.K2_GetActorRotation");

		struct
		{
			FRotator ret;
		} Params;

		ProcessEvent(Target, K2_GetActorRotation, &Params);

		return Params.ret;
	}

	static void SetActorScale3D(UObject* Target, FVector NewScale3D = {})
	{
		static auto SetActorScale3D = FindObject(L"Function /Script/Engine.Actor.SetActorScale3D");

		ProcessEvent(Target, SetActorScale3D, &NewScale3D);
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

namespace Render
{
	static void MapWithActor(UObject* actorClass, std::string map, float actorWidth, float actorHeight, int lineLength, FVector loc, FRotator rot)
	{
		auto location = loc;

		for (std::string::size_type i = 0; i < map.size(); ++i)
		{
			location.X -= actorHeight;

			if (i % lineLength == 0)
			{
				location.Z -= actorWidth;
				location.X = loc.X;
			}

			if (map[i] == '#')
			{
				SpawnActorEasy(Globals::World, actorClass, location, rot);
			}

			//printf("CurrentX: %f, CurrentZ: %f\n", location.X, location.Z);
		}

		location = {};
	}
}

namespace Inventory
{
	static UObject* CreateItem(UObject* ItemDefinition, int Count)
	{
		UObject* TemporaryItemInstance = Player::CreateTemporaryItemInstanceBP(ItemDefinition, Count, 1);

		if (TemporaryItemInstance)
		{
			Player::SetOwningControllerForTemporaryItem(TemporaryItemInstance, Globals::Controller);
		}

		int* CurrentCount = reinterpret_cast<int*>(__int64(TemporaryItemInstance) + __int64(Offsets::ItemEntryOffset) + __int64(0xC));
		*CurrentCount = Count;

		return TemporaryItemInstance;
	}

	static void UpdateInventory()
	{
		static auto HandleWorldInventoryLocalUpdate = FindObject(L"Function /Script/FortniteGame.FortPlayerController.HandleWorldInventoryLocalUpdate");
		static auto HandleInventoryLocalUpdate = FindObject(L"Function /Script/FortniteGame.FortInventory.HandleInventoryLocalUpdate");
		static auto OnRep_QuickBar = FindObject(L"Function /Script/FortniteGame.FortPlayerController.OnRep_QuickBar");
		static auto OnRep_SecondaryQuickBar = FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_SecondaryQuickBar");
		static auto OnRep_PrimaryQuickBar = FindObject(L"Function /Script/FortniteGame.FortQuickBars.OnRep_PrimaryQuickBar");

		ProcessEvent(Globals::FortInventory, HandleInventoryLocalUpdate, nullptr);
		ProcessEvent(Globals::Controller, HandleWorldInventoryLocalUpdate, nullptr);
		ProcessEvent(Globals::Controller, OnRep_QuickBar, nullptr);
		ProcessEvent(Globals::Quickbar, OnRep_SecondaryQuickBar, nullptr);
		ProcessEvent(Globals::Quickbar, OnRep_PrimaryQuickBar, nullptr);
	}

	static int test()
	{
	}

	static void AddItemToInventory(UObject* FortItem, EFortQuickBars QuickbarIndex, int Slot)
	{
		struct ItemEntrySize
		{
			unsigned char Unk00[0xD0]; // Padding. TODO: DO DYNAMICALLY OR EVERYTHING WILL BREAK
		};

		auto ItemInstances = reinterpret_cast<TArray<UObject*>*>(__int64(Globals::FortInventory) + __int64(Offsets::InventoryOffset) + __int64(Offsets::ItemInstancesOffset));
		auto ItemEntries = reinterpret_cast<TArray<ItemEntrySize>*>(__int64(Globals::FortInventory) + __int64(Offsets::InventoryOffset) + __int64(Offsets::ItemEntriesOffset));

		auto ItemEntry = reinterpret_cast<ItemEntrySize*>(reinterpret_cast<uintptr_t>(FortItem) + Offsets::ItemEntryOffset);

		ItemEntries->Add(*ItemEntry);
		ItemInstances->Add(FortItem);

		Player::ServerAddItemInternal(Globals::Quickbar, Player::GetItemGuid(FortItem), QuickbarIndex, Slot);
	}

	static void AddItemToInventoryWithUpdate(UObject* ItemDef, EFortQuickBars QuickbarIndex, int Slot, int Count)
	{
		UObject* ItemInstance = CreateItem(ItemDef, Count);

		if (ItemInstance)
		{
			AddItemToInventory(ItemInstance, QuickbarIndex, Slot);
		}

		UpdateInventory();
	}

}