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
	auto LoadObjectAddress = Util::FindPattern(UE_4_20_LOADOBJECT);
	auto ConstructObjectAddress = Util::FindPattern(UE_4_20_CONSTRUCTOBJECT);
	auto FreeAddress = Util::FindPattern(UE_4_20_FREE);

	if (GObjectsAddress && ToStringAddress && GetFirstPlayerControllerAddress && SpawnActorFromClassAddress && LoadObjectAddress && ConstructObjectAddress && FreeAddress)
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
		LoadObjectAddress = Util::FindPattern(UE_4_21_LOADOBJECT);
		ConstructObjectAddress = Util::FindPattern(UE_4_21_CONSTRUCTOBJECT);
		FreeAddress = Util::FindPattern(UE_4_21_FREE);

		if (GObjectsAddress && ToStringAddress && GetFirstPlayerControllerAddress && SpawnActorFromClassAddress && LoadObjectAddress && ConstructObjectAddress && FreeAddress)
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
	StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(LoadObjectAddress);
	FreeInternal = decltype(FreeInternal)(FreeAddress);
	ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]);

	// we are ready to enter a game
	Game::Setup();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Setup();
	}

	return TRUE;
}
