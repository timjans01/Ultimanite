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

	ObjObjects = decltype(ObjObjects)(ObjectArrayAddress);
	FNameToString = decltype(FNameToString)(ToStringAddress);
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