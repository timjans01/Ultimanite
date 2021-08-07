#pragma once
#include "script_wrappers.h"

namespace UScript
{
	static void error_handler(void* udata, const char* msg) {
		(void)udata;
		std::string message = "*** FATAL ERROR: ";
		message.append(msg ? msg : "NO MESSAGE");
		std::wstring messageW(message.begin(), message.end());

		printf("\n%s\n\n", message.c_str());
		Kismet::Say(messageW.c_str());
		return;
	}

	static void ExecuteStartupScript()
	{
		auto content = Util::readAllText(Util::GetRuntimePath() + "\\uscripts\\startup.js");

		if (content.empty()) return;

		duk_eval_string_noresult(Globals::DukContext, content.c_str());

		return;
	}

	static void F7()
	{
		auto content = Util::readAllText(Util::GetRuntimePath() + "\\uscripts\\f7.js");

		if (content.empty()) return;

		duk_eval_string_noresult(Globals::DukContext, content.c_str());

		return;
	}

	static void InitBindings()
	{
		duk_context* ctx = duk_create_heap(NULL, NULL, NULL, NULL, error_handler);

		Globals::DukContext = ctx;

		duk_push_c_function(ctx, duk_findobject, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UFindObject");

		duk_push_c_function(ctx, duk_spawnactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USpawnActor");

		duk_push_c_function(ctx, duk_destroyactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UDestroyActor");

		duk_push_c_function(ctx, duk_additemtoinventory, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UAddItemToInventory");

		duk_push_c_function(ctx, duk_scaleactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UScaleActor");

		duk_push_c_function(ctx, duk_getlocalplayer, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UGetLocalPlayer");

		duk_push_c_function(ctx, duk_getactorofclass, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UGetActorOfClass");
		
		duk_push_c_function(ctx, duk_getactorlocation, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UGetActorLocation");

		duk_push_c_function(ctx, duk_displayobjectname, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UDisplayObjectName");

		duk_push_c_function(ctx, duk_teleportactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UTeleportActor");

		duk_push_c_function(ctx, duk_spawnpickupatlocation, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USpawnPickupAtLocation");

		duk_push_c_function(ctx, duk_spawntextactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USpawnTextActor");

		duk_push_c_function(ctx, duk_settextactortext, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USetTextActorText");

		duk_push_c_function(ctx, duk_activateability, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, _("UActivateAbility"));

		duk_push_c_function(ctx, duk_renderasciiwithactor, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "URenderASCIIWithActor");

		duk_push_c_function(ctx, duk_webclient, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UWebClient");

		duk_push_c_function(ctx, duk_webclientget, DUK_VARARGS);//DOC
		duk_put_global_string(ctx, "UWebClientGet");

		duk_push_c_function(ctx, duk_webclientpost, DUK_VARARGS);//DOC
		duk_put_global_string(ctx, "UWebClientPost");

		duk_push_c_function(ctx, duk_spawnbot, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USpawnBot");

		duk_push_c_function(ctx, duk_movebottotarget, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UMoveBotToTarget");

		duk_push_c_function(ctx, duk_setplayermaxhealth, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USetPlayerMaxHealth");

		duk_push_c_function(ctx, duk_setplayerhealth, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USetPlayerHealth");

		duk_push_c_function(ctx, duk_setplayermaxshield, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USetPlayerMaxShield");

		duk_push_c_function(ctx, duk_setplayershield, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "USetPlayerShield");

		duk_push_c_function(ctx, duk_executeconsolecommand, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UExecuteConsoleCommand");

		duk_push_c_function(ctx, duk_readfileasstring, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UReadFileAsString");

		duk_push_c_function(ctx, duk_getgamepath, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UGetGamePath");

		duk_push_c_function(ctx, duk_print, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UPrint");

		duk_push_c_function(ctx, duk_jump, DUK_VARARGS); //DOC
		duk_put_global_string(ctx, "UJump");

		/*duk_push_c_function(ctx, duk_triggerwin, DUK_VARARGS);
		duk_put_global_string(ctx, "UTriggerWin");

		duk_push_c_function(ctx, duk_processeventhook, DUK_VARARGS);
		duk_put_global_string(ctx, "UProcessEventHook");*/
	}
}
