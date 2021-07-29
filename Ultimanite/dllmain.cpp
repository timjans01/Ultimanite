#include "framework.h"
#include "gameplay.h"
#include "offsets.h"

void Setup()
{
	Util::SetupConsole();

	EEngineVersion CurrentVersion = EEngineVersion::None;

	auto GObjectsAddress = Util::FindPattern(UE_4_20_GOBJECTS);
	auto ToStringAddress = Util::FindPattern(UE_4_20_FNAME_TOSTRING);
	auto GetFirstPlayerControllerAddress = Util::FindPattern(UE_4_20_GETFIRSTPLAYERCONTROLLER);
	auto SpawnActorFromClassAddress = Util::FindPattern(UE_4_20_SPAWNACTORFROMCLASS);
	auto ConstructObjectAddress = Util::FindPattern(UE_4_20_CONSTRUCTOBJECT);

	if (GObjectsAddress && ToStringAddress && GetFirstPlayerControllerAddress && SpawnActorFromClassAddress && ConstructObjectAddress)
	{
		CurrentVersion = EEngineVersion::UE_4_20;
	}

	// not able to find any version yet, lets try again
	if (CurrentVersion == EEngineVersion::None)
	{
		GObjectsAddress = Util::FindPattern(UE_4_21_GOBJECTS);
		ToStringAddress = Util::FindPattern(UE_4_21_FNAME_TOSTRING);
		GetFirstPlayerControllerAddress = Util::FindPattern(UE_4_21_GETFIRSTPLAYERCONTROLLER);
		SpawnActorFromClassAddress = Util::FindPattern(UE_4_21_SPAWNACTORFROMCLASS);
		ConstructObjectAddress = Util::FindPattern(UE_4_21_CONSTRUCTOBJECT);

		if (GObjectsAddress && ToStringAddress && GetFirstPlayerControllerAddress && SpawnActorFromClassAddress && ConstructObjectAddress)
		{
			CurrentVersion = EEngineVersion::UE_4_21;
		}
	}

	if (CurrentVersion == EEngineVersion::None)
	{
		printf("Unsupported Engine version!\n");
		return;
	}

	auto ObjectsOffset = *(int32_t*)(GObjectsAddress + 3);
	auto FinalObjectsAddress = GObjectsAddress + 7 + ObjectsOffset;

	if (CurrentVersion >= EEngineVersion::UE_4_21)
	{
		// support recent version of GObjects
		GlobalObjects = decltype(GlobalObjects)(FinalObjectsAddress);
	}
	else
	{
		// support legacy version of GObjects
		ObjObjects = decltype(ObjObjects)(FinalObjectsAddress);
	}

	FNameToString = decltype(FNameToString)(ToStringAddress);
	GetFirstPlayerController = decltype(GetFirstPlayerController)(GetFirstPlayerControllerAddress);
	SpawnActor = decltype(SpawnActor)(SpawnActorFromClassAddress);
	StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(ConstructObjectAddress);
	ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]);

	// we are ready to enter a game
	Game::Setup();

	/*
	auto UE_4_20_GObjectsAddress = Util::FindPattern(UE_4_20_GOBJECTS);
	auto UE_4_20_ToStringAddress = Util::FindPattern(UE_4_20_FNAME_TOSTRING);
	auto UE_4_20_GetFirstPlayerController = Util::FindPattern(UE_4_20_GETFIRSTPLAYERCONTROLLER);
	auto UE_4_20_SpawnActorFromClass = Util::FindPattern(UE_4_20_SPAWNACTORFROMCLASS);
	auto UE_4_20_ConstructObject = Util::FindPattern(UE_4_20_CONSTRUCTOBJECT);

	if (UE_4_20_GObjectsAddress && UE_4_20_ToStringAddress && UE_4_20_GetFirstPlayerController && UE_4_20_SpawnActorFromClass && UE_4_20_ConstructObject)
	{
		auto offset = *(int32_t*)(UE_4_20_GObjectsAddress + 3);
		auto fnAddress = UE_4_20_GObjectsAddress + 7 + offset;
		
		ObjObjects = decltype(ObjObjects)(fnAddress);
		FNameToString = decltype(FNameToString)(UE_4_20_ToStringAddress);
		GetFirstPlayerController = decltype(GetFirstPlayerController)(UE_4_20_GetFirstPlayerController);
		SpawnActor = decltype(SpawnActor)(UE_4_20_SpawnActorFromClass);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(UE_4_20_ConstructObject);
		ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]);

		printf("UE Version: 4.20\n");

		// load into game
		Game::Setup();

		return;
	}

	auto UE_4_21_GObjectsAddress = Util::FindPattern(UE_4_21_GOBJECTS);
	auto UE_4_21_ToStringAddress = Util::FindPattern(UE_4_21_FNAME_TOSTRING);
	auto UE_4_21_GetFirstPlayerController = Util::FindPattern(UE_4_21_GETFIRSTPLAYERCONTROLLER);
	auto UE_4_21_SpawnActorFromClass = Util::FindPattern(UE_4_21_SPAWNACTORFROMCLASS);
	auto UE_4_21_ConstructObject = Util::FindPattern(UE_4_21_CONSTRUCTOBJECT);

	if (!UE_4_21_GObjectsAddress) printf("UE_4_21_GObjectsAddress\n");
	if (!UE_4_21_ToStringAddress) printf("UE_4_21_ToStringAddress\n");
	if (!UE_4_21_GetFirstPlayerController) printf("UE_4_21_GetFirstPlayerController\n");
	if (!UE_4_21_SpawnActorFromClass) printf("UE_4_21_SpawnActorFromClass\n");
	if (!UE_4_21_ConstructObject) printf("UE_4_21_ConstructObject\n");

	if (UE_4_21_GObjectsAddress && UE_4_21_ToStringAddress && UE_4_21_GetFirstPlayerController && UE_4_21_SpawnActorFromClass && UE_4_21_ConstructObject)
	{
		auto offset = *(int32_t*)(UE_4_21_GObjectsAddress + 3);
		auto fnAddress = UE_4_21_GObjectsAddress + 7 + offset;

		GlobalObjects = decltype(GlobalObjects)(fnAddress);
		FNameToString = decltype(FNameToString)(UE_4_21_ToStringAddress);
		GetFirstPlayerController = decltype(GetFirstPlayerController)(UE_4_21_GetFirstPlayerController);
		SpawnActor = decltype(SpawnActor)(UE_4_21_SpawnActorFromClass);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(UE_4_21_ConstructObject);
		ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]); // could change

		printf("UE Version: 4.21\n");

		Game::Setup();
	}
	*/
	
	//Game::Setup();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Setup();
	}

	return TRUE;
}
