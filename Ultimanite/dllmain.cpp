#include "framework.h"
#include "gameplay.h"
#include "offsets.h"

void Setup()
{
	Util::SetupConsole();

	// UE VERSION 4.20
	
	/*auto UE_4_20_GObjectsAddress = Util::FindPattern(UE_4_20_GOBJECTS);
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
		ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]); // could change

		printf("UE Version: 4.20\n");

		// load into game
		// Game::Setup();

		return;
	}
	// END UE VERSION 4.20
	*/


	// START UE VERSION 4.21
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

		auto World = GetWorld();

		if (World)
		{
			printf("World: %ws\n", World->GetFullName().c_str());

			auto Controller = GetFirstPlayerController(World);

			if (Controller)
			{
				printf("Controller: %ws\n", Controller->GetFullName().c_str());

				Game::Setup();
			}
			else
			{
				printf("No controller\n");
			}
		}
		else
		{
			printf("No world\n");
		}
	}
	else
	{
		printf("No UE version\n");
	}
	
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
