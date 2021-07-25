#pragma once
#include "framework.h"

inline TUObjectArray* ObjObjects;

inline UObject* (*GetFirstPlayerController)(UObject* World);

inline UObject* (*SpawnActor)(UObject* World, UObject* Class, FVector* Position, FRotator* Rotation, const FActorSpawnParameters& SpawnParameters);

static UObject* (*StaticConstructObjectInternal)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool);

static UObject* (*StaticLoadObjectInternal)(UObject* ObjectClass, UObject* InOuter, const TCHAR* InName, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation);

inline void (*TickPlayerInput)(const UObject* PlayerController, const float DeltaSeconds, const bool bGamePaused);


static UObject* FindObjectById(uint32_t Id)
{
	auto Offset = 24 * Id;
	return *(UObject**)(ObjObjects->Objects + Offset);
}

template <typename T = UObject*> static T FindObject(std::wstring ObjectToFind)
{
	for (int i = 0; i < ObjObjects->NumElements; i++)
	{
		auto Object = FindObjectById(i);

		if (Object == nullptr)
		{
			continue;
		}

		//I am aware this is bad but we can't move to starts_with since the game itself is using an old std version.
		if (wcsstr(Object->GetFullName().c_str(), ObjectToFind.c_str()))
		{
			return (T)Object;
		}
	}

	return nullptr;
}

static DWORD FindOffset(std::wstring OffsetToFind)
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

static UObject* GetWorld()
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

static UObject* SpawnActorEasy(UObject* WorldContextObject, UObject* Actor, FVector Location, FRotator ParamRotation)
{
	FRotator Rotation = ParamRotation;
	return SpawnActor(WorldContextObject, Actor, &Location, &Rotation, FActorSpawnParameters());
}

static UObject* StaticLoadObjectEasy(UObject* ObjectClass, const wchar_t* InPath)
{
	return StaticLoadObjectInternal(ObjectClass, nullptr, InPath, nullptr, 0, nullptr, false);
}
