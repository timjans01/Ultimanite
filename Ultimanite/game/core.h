#pragma once

#include <detours.h>

#include "sdk.h"

namespace Game
{
	bool bReady = false;
	bool bJumpedFromAircraft = false;
	
	void LoadMatch()
	{
		Globals::Controller = FindObject(L"Athena_PlayerController_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_PlayerController_C");
		Globals::GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		Globals::GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");

		Globals::CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Globals::Controller, 0, 0, 0, 0, 0, 0, 0);

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{29481.783, 40562.594, 1237.150});

		Player::Possess(Globals::Controller, Globals::Pawn);

		struct AthenaGameState
		{
			unsigned char Unk00[0x1cb0];
			EAthenaGamePhase GamePhase;
		};

		((AthenaGameState*)Globals::GameState)->GamePhase = EAthenaGamePhase::Warmup;

		GameState::OnRep_GamePhase(Globals::GameState, EAthenaGamePhase::None);

		Player::ServerReadyToStartMatch(Globals::Controller);
	}

	void JumpFromAircraft()
	{
		if (Globals::Pawn)
		{
			AActor::Destroy(Globals::Pawn);
		}
		CheatManager::Summon(Globals::CheatManager, L"PlayerPawn_Athena_C");
		Globals::Pawn = GameplayStatics::GetAllActorsOfClass(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C")[0];
		Player::Possess(Globals::Controller, Globals::Pawn);

		//TODO: Fix rotation
	}

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

	void Setup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(void*&)ProcessEvent, ProcessEventDetour);
		DetourTransactionCommit();

		auto PlayerController = GetFirstPlayerController(GetWorld());

		if (PlayerController)
		{
			Player::SwitchLevel(PlayerController, L"Athena_Terrain?Game=/Game/Athena/Athena_GameMode.Athena_GameMode_C");
		}
	}
}