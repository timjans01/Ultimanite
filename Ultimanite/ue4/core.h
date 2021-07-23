#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "enums.h"
#include "structs.h"

inline TUObjectArray* ObjObjects;

inline void* (*ProcessEvent)(UObject* Object, UObject* Function, void* Params);

inline UObject* (*GetFirstPlayerController)(UObject* World);

inline UObject* (*SpawnActor)(UObject* World, UObject* Class, FVector* Position, FRotator* Rotation, const FActorSpawnParameters& SpawnParameters);

inline UObject* (*StaticConstructObjectInternal)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool);

inline UObject* FindObjectById(uint32_t Id)
{
	auto Offset = 24 * Id;
	return *(UObject**)(ObjObjects->Objects + Offset);
}

inline UObject* FindObject(std::wstring ObjectToFind)
{
	for (int i = 0; i < ObjObjects->NumElements; i++)
	{
		auto Object = FindObjectById(i);

		if (Object == nullptr)
		{
			continue;
		}

		if (wcsstr(Object->GetFullName().c_str(), ObjectToFind.c_str()))
		{
			return Object;
		}
	}

	return nullptr;
}

inline DWORD FindOffset(std::wstring OffsetToFind)
{
	auto Object = FindObject(OffsetToFind);

	if (Object)
	{
		return *(uint32_t*)(__int64(Object) + 0x44);
	}

	return 0;
}

inline void DumpObjects()
{
	std::wofstream DumpFile("dump_objects_log.txt");

	for (int i = 0; i < ObjObjects->NumElements; i++)
	{
		auto Object = FindObjectById(i);

		if (Object == nullptr)
		{
			continue;
		}

		DumpFile << Object->GetFullName() << std::endl;
	}
}

inline UObject* GetWorld()
{
	auto FortEngine = FindObject(L"FortEngine_");

	struct Viewport
	{
		unsigned char Unk00[0x88];
		UObject* World;
	};

	struct Engine
	{
		unsigned char Unk00[0x728];
		Viewport* GameViewport;
	};

	return ((Engine*)FortEngine)->GameViewport->World;
}

inline UObject* SpawnActorEasy(UObject* WorldContextObject, UObject* Actor, FVector Location)
{
	FRotator Rotation{};
	return SpawnActor(WorldContextObject, Actor, &Location, &Rotation, FActorSpawnParameters());
}