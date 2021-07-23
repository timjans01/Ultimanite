#pragma once

#include <detours.h>

#include "sdk.h"

namespace Game
{
	bool bReady = false;

	void LoadMatch()
	{
		UObject* Controller = FindObject(L"Athena_PlayerController_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_PlayerController_C");
		UObject* GameState = FindObject(L"Athena_GameState_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameState_C");
		UObject* GameMode = FindObject(L"Athena_GameMode_C /Game/Athena/Maps/Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_GameMode_C");

		UObject* CheatManager = StaticConstructObjectInternal(FindObject(L"Class /Script/Engine.CheatManager"), Controller, 0, 0, 0, 0, 0, 0, 0);
		CheatManager::Summon(CheatManager, L"PlayerPawn_Athena_C");

		UObject* Pawn = FindObject(L"PlayerPawn_Athena_C_");

		Player::Possess(Controller, Pawn);

		struct TestGameState
		{
			unsigned char Unk00[0x1cb0];
			EAthenaGamePhase GamePhase;
		};

		((TestGameState*)GameState)->GamePhase = EAthenaGamePhase::Warmup;

		GameState::OnRep_GamePhase(GameState, EAthenaGamePhase::None);

		Player::ServerReadyToStartMatch(Controller);
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