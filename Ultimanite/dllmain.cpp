#include "framework.h"
#include "gameplay.h"

void Setup()
{
	Util::SetupConsole();

	auto ModuleBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

	// TODO: Move to patterns
	auto ToStringAddress = ModuleBaseAddress + CONSTS::FNAME_TO_STRING_OFFSET;
	auto ObjectArrayAddress = ModuleBaseAddress + CONSTS::OBJECTS_ARRAY_OFFSET;
	auto GetFirstPlayerControllerAddress = ModuleBaseAddress + CONSTS::GET_FIRST_CONTROLLER_OFFSET;
	auto SpawnActorAddress = ModuleBaseAddress + CONSTS::SPAWN_ACTOR_OFFSET;
	auto StaticConstructObjectInternalAddress = ModuleBaseAddress + CONSTS::STATIC_CONSTRUCT_OBJECT_INTERNAL_OFFSET;

	ObjObjects = decltype(ObjObjects)(ObjectArrayAddress);
	FNameToString = decltype(FNameToString)(ToStringAddress);
	GetFirstPlayerController = decltype(GetFirstPlayerController)(GetFirstPlayerControllerAddress);
	SpawnActor = decltype(SpawnActor)(SpawnActorAddress);
	StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(StaticConstructObjectInternalAddress);
	ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[CONSTS::PROCESS_EVENT_INDEX]);

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
