#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <string_view>

#include "structs.h"
#include "enums.h"

inline TUObjectArray* ObjObjects;

inline void* (*ProcessEvent)(UObject* Object, UObject* Function, void* Params);

UObject* FindObjectById(uint32_t Id)
{
	auto Offset = 24 * Id;
	return *(UObject**)(ObjObjects->Objects + Offset);
}

UObject* FindObject(std::wstring ObjectToFind)
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

DWORD FindOffset(std::wstring OffsetToFind)
{
	auto Object = FindObject(OffsetToFind);

	if (Object)
	{
		return *(uint32_t*)(__int64(Object) + 0x44);
	}

	return 0;
}