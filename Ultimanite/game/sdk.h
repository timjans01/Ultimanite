#pragma once

namespace Player
{
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
}

namespace GameState
{
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