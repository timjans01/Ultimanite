#pragma once
#include "framework.h"

namespace Game
{
	bool bReady = false;
	bool bJumpedFromAircraft = false;
	
	static void LoadMatch()
	{
		Globals::Controller = FindObject(L"Athena_PlayerController_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_PlayerController_C");
		Globals::GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		Globals::GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");

		Globals::CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Globals::Controller, 0, 0, 0, 0, 0, 0, 0);

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{35000, 40562.594, 1300.150});

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

	static void JumpFromAircraft()
	{
		auto bus = GameState::GetAircraft(Globals::GameState);

		auto loc = AActor::GetLocation(bus);

		if (Globals::Pawn && Globals::Pawn->isValid())
		{
			AActor::Destroy(Globals::Pawn);
		}

		Globals::Pawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), loc);
		Player::Possess(Globals::Controller, Globals::Pawn);
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
			static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");

			PlayerController->Call(SwitchLevel, FString(L"Athena_Terrain?Game=/Game/Athena/Athena_GameMode.Athena_GameMode_C"));
		}
	}
}