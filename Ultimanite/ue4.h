#pragma once
#include "framework.h"

// Used on UE version 4.20 and lower.
inline TUObjectArray* ObjObjects;

// Used on UE version 4.21 and higher.
inline GObjects* GlobalObjects;


inline UObject* (*GetFirstPlayerController)(UObject* World);

inline UObject* (*SpawnActor)(UObject* World, UObject* Class, FVector* Position, FRotator* Rotation, const FActorSpawnParameters& SpawnParameters);

static UObject* (*StaticConstructObjectInternal)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool);

static UObject* (*StaticLoadObjectInternal)(UObject* ObjectClass, UObject* InOuter, const TCHAR* InName, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation);

inline void (*TickPlayerInput)(const UObject* PlayerController, const float DeltaSeconds, const bool bGamePaused);

enum class EEngineVersion : uint8_t
{
	UE_4_20 = 0,
	UE_4_21 = 1,
	None = 2
};

inline void NumChunks(int* start, int* end)
{
	int cStart = 0, cEnd = 0;

	if (!cEnd)
	{
		while (1)
		{
			if (GlobalObjects->ObjectArray->Objects[cStart] == 0)
			{
				cStart++;
			}
			else
			{
				break;
			}
		}

		cEnd = cStart;
		while (1)
		{
			if (GlobalObjects->ObjectArray->Objects[cEnd] == 0)
			{
				break;
			}
			else
			{
				cEnd++;
			}
		}
	}

	*start = cStart;
	*end = cEnd;
}



static UObject* FindObjectById(uint32_t Id)
{
	if (GlobalObjects)
	{
		// we are on ue 4.21+
		int cStart = 0, cEnd = 0;
		int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
		FUObjectItem* Object;

		NumChunks(&cStart, &cEnd);

		chunkIndex = Id / chunkSize;
		if (chunkSize * chunkIndex != 0 &&
			chunkSize * chunkIndex == Id)
		{
			chunkIndex--;
		}

		chunkPos = cStart + chunkIndex;
		if (chunkPos < cEnd)
		{
			Object = GlobalObjects->ObjectArray->Objects[chunkPos] + (Id - chunkSize * chunkIndex);

			if (!Object) { return NULL; }

			return Object->Object;
		}
	}
	else
	{
		// we are on ue 4.20 on lower
		auto Offset = 24 * Id;
		return *(UObject**)(ObjObjects->Objects + Offset);
	}

	return nullptr;
}

template <typename T = UObject*> static T FindObject(std::wstring ObjectToFind, bool IsEqual = false)
{
	int ObjectCount = GlobalObjects ? GlobalObjects->ObjectCount : ObjObjects->NumElements;

	for (int i = 0; i < ObjectCount; i++)
	{
		auto Object = FindObjectById(i);

		if (Object == nullptr)
		{
			continue;
		}

		if (IsEqual)
		{
			if (Object->GetFullName() == ObjectToFind)
			{
				return (T)Object;
			}
		}
		else
		{
			if (wcsstr(Object->GetFullName().c_str(), ObjectToFind.c_str()))
			{
				return (T)Object;
			}
		}
	}

	return nullptr;
}

static DWORD FindOffset(std::wstring OffsetToFind)
{
	auto Object = FindObject(OffsetToFind, true);

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
	static auto FortEngine = FindObject(L"FortEngine_");
	static auto GameViewportOffset = FindOffset(L"ObjectProperty /Script/Engine.Engine.GameViewport");
	static auto WorldOffset = FindOffset(L"ObjectProperty /Script/Engine.GameViewportClient.World");

	UObject* GameViewport = *reinterpret_cast<UObject**>(__int64(FortEngine) + __int64(GameViewportOffset));
	UObject** World = reinterpret_cast<UObject**>(__int64(GameViewport) + __int64(WorldOffset));

	return *World;
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
