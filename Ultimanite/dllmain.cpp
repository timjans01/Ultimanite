#include "framework.h"
#include "gameplay.h"
#include "offsets.h"

void Setup()
{
	Util::SetupConsole();

	auto UE_4_20_GObjectsAddress = Util::FindPattern(UE_4_20_GOBJECTS);
	auto UE_4_20_ToStringAddress = Util::FindPattern(UE_4_20_FNAME_TOSTRING);
	auto UE_4_20_GetFirstPlayerController = Util::FindPattern(UE_4_20_GETFIRSTPLAYERCONTROLLER);
	auto UE_4_20_SpawnActorFromClass = Util::FindPattern(UE_4_20_SPAWNACTORFROMCLASS);
	auto UE_4_20_ConstructObject = Util::FindPattern(UE_4_20_CONSTRUCTOBJECT);

	if (!UE_4_20_GObjectsAddress) printf("Not found: UE_4_20_GObjectsAddress\n");
	if (!UE_4_20_ToStringAddress) printf("Not found: UE_4_20_ToStringAddress\n");
	if (!UE_4_20_GetFirstPlayerController) printf("Not found: UE_4_20_GetFirstPlayerController\n");
	if (!UE_4_20_SpawnActorFromClass) printf("Not found: UE_4_20_SpawnActorFromClass\n");
	if (!UE_4_20_ConstructObject) printf("Not found: UE_4_20_ConstructObject\n");

	// we have been injected into a build running 4.20
	if (UE_4_20_GObjectsAddress &&
		UE_4_20_ToStringAddress &&
		UE_4_20_GetFirstPlayerController &&
		UE_4_20_SpawnActorFromClass &&
		UE_4_20_ConstructObject)
	{
		auto offset = *(int32_t*)(UE_4_20_GObjectsAddress + 3);
		auto fnAddress = UE_4_20_GObjectsAddress + 7 + offset;
		
		ObjObjects = decltype(ObjObjects)(fnAddress);
		FNameToString = decltype(FNameToString)(UE_4_20_ToStringAddress);
		GetFirstPlayerController = decltype(GetFirstPlayerController)(UE_4_20_GetFirstPlayerController);
		SpawnActor = decltype(SpawnActor)(UE_4_20_SpawnActorFromClass);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(UE_4_20_ConstructObject);
		ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]); // could change

		printf("Setup everything for UE 4.20!\n");
	}


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
