#pragma once

#include "script_wrappers.h"

namespace UScript
{
	static void error_handler(void* udata, const char* msg)
	{
		(void)udata;
		printf("\n*** FATAL ERROR: %s\n%s", (msg ? msg : "no message"), "\n");
		return;
	}

	static void SetupBindings()
	{
		duk_context* ctx = duk_create_heap(NULL, NULL, NULL, NULL, error_handler);

		Globals::DukContext = ctx;

		duk_push_c_function(ctx, duk_findobject, DUK_VARARGS);
		duk_put_global_string(ctx, "UFindObject");

		duk_push_c_function(ctx, duk_spawnactor, DUK_VARARGS);
		duk_put_global_string(ctx, "USpawnActor");

		duk_push_c_function(ctx, duk_destroyactor, DUK_VARARGS);
		duk_put_global_string(ctx, "UDestroyActor");

		duk_push_c_function(ctx, duk_additemtoinventory, DUK_VARARGS);
		duk_put_global_string(ctx, "UAddItemToInventory");

		duk_push_c_function(ctx, duk_scaleactor, DUK_VARARGS);
		duk_put_global_string(ctx, "UScaleActor");

		duk_push_c_function(ctx, duk_getactorofclass, DUK_VARARGS);
		duk_put_global_string(ctx, "UGetActorOfClass");

		duk_push_c_function(ctx, duk_displayobjectname, DUK_VARARGS);
		duk_put_global_string(ctx, "UDisplayObjectName");

		duk_push_c_function(ctx, duk_teleportactor, DUK_VARARGS);
		duk_put_global_string(ctx, "UTeleportActor");

		duk_push_c_function(ctx, duk_spawnpickupatlocation, DUK_VARARGS);
		duk_put_global_string(ctx, "USpawnPickupAtLocation");

		duk_push_c_function(ctx, duk_spawntextactor, DUK_VARARGS);
		duk_put_global_string(ctx, "USpawnTextActor");

		duk_push_c_function(ctx, duk_settextactortext, DUK_VARARGS);
		duk_put_global_string(ctx, "USetTextActorText");

		duk_push_c_function(ctx, duk_activateability, DUK_VARARGS);
		duk_put_global_string(ctx, "UActivateAbility");

		duk_push_c_function(ctx, duk_renderasciiwithactor, DUK_VARARGS);
		duk_put_global_string(ctx, "URenderASCIIWithActor");

		duk_push_c_function(ctx, duk_webclient, DUK_VARARGS);
		duk_put_global_string(ctx, "UWebClient");

		duk_push_c_function(ctx, duk_webclientget, DUK_VARARGS);
		duk_put_global_string(ctx, "UWebClientGet");
	}
}