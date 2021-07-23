#include "util.h"
#include "ue4/core.h"
#include "game/core.h"

void Setup()
{
	SetupConsole();

	auto ModuleBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

	// TODO: Move to patterns
	auto ToStringAddress = ModuleBaseAddress + 0x13C37C0;
	auto ObjectArrayAddress = ModuleBaseAddress + 0x4D01930;
	auto GetFirstPlayerControllerAddress = ModuleBaseAddress + 0x25EE0F0;
	auto SpawnActorAddress = ModuleBaseAddress + 0x22DDBF0;
	auto StaticConstructObjectInternalAddress = ModuleBaseAddress + 0x156EC20;

	ObjObjects = decltype(ObjObjects)(ObjectArrayAddress);
	FNameToString = decltype(FNameToString)(ToStringAddress);
	GetFirstPlayerController = decltype(GetFirstPlayerController)(GetFirstPlayerControllerAddress);
	SpawnActor = decltype(SpawnActor)(SpawnActorAddress);
	StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(StaticConstructObjectInternalAddress);
	ProcessEvent = decltype(ProcessEvent)(FindObject(L"FortEngine_")->VTableObject[0x40]);

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