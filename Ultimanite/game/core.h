#pragma once

#include <detours.h>

namespace Game
{
	void* ProcessEventDetour(UObject* Object, UObject* Function, void* Params)
	{
		auto ObjectName = Object->GetFullName();
		auto FunctionName = Function->GetFullName();

		printf("%ws", FunctionName.c_str());

		return ProcessEvent(Object, Function, Params);
	}

	void Setup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(void*&)ProcessEvent, ProcessEventDetour);

		DetourTransactionCommit();
	}
}