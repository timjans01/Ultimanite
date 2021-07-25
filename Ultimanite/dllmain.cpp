#include "framework.h"
#include "gameplay.h"

static __forceinline uintptr_t FindPattern(const char* signature, bool bRelative = false, uint32_t offset = 0)
{
    uintptr_t base_address = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    static auto patternToByte = [](const char* pattern)
    {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else { bytes.push_back(strtoul(current, &current, 16)); }
        }
        return bytes;
    };

    const auto dosHeader = (PIMAGE_DOS_HEADER)base_address;
    const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)base_address + dosHeader->e_lfanew);

    const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = patternToByte(signature);
    const auto scanBytes = reinterpret_cast<std::uint8_t*>(base_address);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool found = true;
        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            uintptr_t address = reinterpret_cast<uintptr_t>(&scanBytes[i]);
            if (bRelative)
            {
                address = ((address + offset + 4) + *(int32_t*)(address + offset));
                return address;
            }
            return address;
        }
    }
    return NULL;
}

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
	auto TickPlayerInputAddress = ModuleBaseAddress + CONSTS::TICK_PLAYER_INPUT_OFFSET;

	ObjObjects = decltype(ObjObjects)(ObjectArrayAddress);
	FNameToString = decltype(FNameToString)(ToStringAddress);
	GetFirstPlayerController = decltype(GetFirstPlayerController)(GetFirstPlayerControllerAddress);
	SpawnActor = decltype(SpawnActor)(SpawnActorAddress);
	StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(StaticConstructObjectInternalAddress);
	TickPlayerInput = decltype(TickPlayerInput)(TickPlayerInputAddress);
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
