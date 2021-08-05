#pragma once
#include "framework.h"

//void UDisplayObjectName(ObjectPointer);
static duk_ret_t duk_displayobjectname(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 1)
	{
		MessageBox(nullptr, L"This function takes 1 arguments!.", L"UDisplayObjectName", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto object = (UObject*)duk_get_pointer(ctx, 0);

	if (!object || Util::IsBadReadPtr(object))
	{
		MessageBox(nullptr, L"Object is null.", L"UDisplayObjectName", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto objectNameW = object->GetFullName();

	MessageBoxW(nullptr, objectNameW.c_str(), L"UDisplayObjectName", MB_OK);

	return 0;
}

//UObject* UFindObject("OBJECT FULLNAME");
static duk_ret_t duk_findobject(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 1)
	{
		MessageBox(nullptr, L"This function takes 1 arguments!.", L"UFindObject", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string objectName = duk_get_string(ctx, 0);

	std::wstring objectNameW(objectName.begin(), objectName.end());

	auto object = FindObject(objectNameW);

	if (!object || Util::IsBadReadPtr(object))
	{
		MessageBox(nullptr, L"Object cannot be found.", L"UFindObject", 0);
		return DUK_RET_TYPE_ERROR;
	}

	duk_push_pointer(ctx, object);

	return 1; //one return value
}

//UObject* USpawnActor(ClassObject, [X, Y, Z], [Pitch, Yaw, Roll]);
static duk_ret_t duk_spawnactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 3)
	{
		MessageBox(nullptr, L"This function takes 3 arguments!.", L"USpawnActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto classObject = duk_get_pointer(ctx, 0);

	if (!classObject || Util::IsBadReadPtr(classObject))
	{
		MessageBox(nullptr, L"Actor class was not found, mostly a wrong name.", L"USpawnActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto locationArraySize = duk_get_length(ctx, 1);

	auto rotationArraySize = duk_get_length(ctx, 2);

	if (rotationArraySize == 3 && locationArraySize == 3)
	{
		duk_get_prop_index(ctx, 1, 0);
		auto x = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 1);
		auto y = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 2);
		auto z = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 0);
		auto pitch = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 1);
		auto yaw = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 2);
		auto roll = duk_get_int(ctx, -1);

		FVector Location{x, y, z};
		FRotator Rotation{pitch, yaw, roll};

		auto actor = SpawnActorEasy(GetWorld(), (UObject*)classObject, Location, Rotation);

		if (!actor || Util::IsBadReadPtr(actor))
		{
			MessageBox(nullptr, L"Failed to spawn actor.", L"USpawnActor", 0);
			return DUK_RET_TYPE_ERROR;
		}

		duk_push_pointer(ctx, actor);
	}
	else
	{
		MessageBox(nullptr, L"Location/Rotations is not correct.", L"USpawnActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 1;
}

//void UDestroyActor(ObjectPointer);
static duk_ret_t duk_destroyactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 1)
	{
		MessageBox(nullptr, L"This function takes 1 arguments!.", L"UDestroyActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto object = (UObject*)duk_get_pointer(ctx, 0);

	if (!object || Util::IsBadReadPtr(object))
	{
		MessageBox(nullptr, L"Object is null.", L"UDestroyActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	AActor::Destroy(object);

	return 0;
}

//UObject* UGetActorOfClass(actorClass, index);
static duk_ret_t duk_getactorofclass(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"UGetActorOfClass", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto classObject = (UObject*)duk_get_pointer(ctx, 0);

	if (!classObject || Util::IsBadReadPtr(classObject))
	{
		MessageBox(nullptr, L"Actor class was not found.", L"UGetActorOfClass", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto index = duk_get_int(ctx, 1);

	auto actor = GameplayStatics::GetAllActorsOfClass(classObject)[index];

	if (!actor || Util::IsBadReadPtr(actor))
	{
		MessageBox(nullptr, L"Actor object is null!.", L"UGetActorOfClass", 0);
		return DUK_RET_TYPE_ERROR;
	}

	duk_push_pointer(ctx, actor);

	return 1;
}

//void UScaleActor(ActorObjectPointer, x, y, z);
static duk_ret_t duk_scaleactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 4)
	{
		MessageBox(nullptr, L"This function takes 4 arguments!.", L"UScaleActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* actorObject = (UObject*)duk_get_pointer(ctx, 0);

	if (!actorObject || Util::IsBadReadPtr(actorObject))
	{
		MessageBox(nullptr, L"Object was not found.", L"UScaleActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto x = static_cast<float>(duk_get_int(ctx, 1));
	auto y = static_cast<float>(duk_get_int(ctx, 2));
	auto z = static_cast<float>(duk_get_int(ctx, 3));

	AActor::SetActorScale3D(actorObject, FVector{x, y, z});

	return 0;
}

//void UTeleportActor(objectPointer, [X, Y, Z], [Pitch, Yaw, Roll]);
static duk_ret_t duk_teleportactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 3)
	{
		MessageBox(nullptr, L"This function takes 3 arguments!.", L"UTeleportActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto actor = duk_get_pointer(ctx, 0);

	if (!actor || Util::IsBadReadPtr(actor))
	{
		MessageBox(nullptr, L"Actor was not found.", L"UTeleportActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto locationArraySize = duk_get_length(ctx, 1);

	auto rotationArraySize = duk_get_length(ctx, 2);

	if (rotationArraySize == 3 && locationArraySize == 3)
	{
		duk_get_prop_index(ctx, 1, 0);
		auto x = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 1);
		auto y = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 2);
		auto z = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 0);
		auto pitch = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 1);
		auto yaw = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 2, 2);
		auto roll = duk_get_int(ctx, -1);

		FVector Location{x, y, z};
		FRotator Rotation{pitch, yaw, roll};

		struct Params
		{
			FVector DestLocation;
			FRotator DestRotation;
		};

		Params params;

		params.DestLocation = Location;
		params.DestRotation = Rotation;

		auto func = FindObject(L"Function /Script/Engine.Actor.K2_TeleportTo");

		ProcessEvent(actor, func, &params);
	}
	else
	{
		MessageBox(nullptr, L"Location/Rotations is not correct.", L"UTeleportActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 0;
}

//void UAddItemToInventory(weaponObject, slot);
static duk_ret_t duk_additemtoinventory(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"UAddItemToInventory", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* weaponObject = (UObject*)duk_get_pointer(ctx, 0);

	if (!weaponObject || Util::IsBadReadPtr(weaponObject))
	{
		MessageBox(nullptr, L"Weapons object was not found, mostly a wrong name.", L"UAddItemToInventory", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto slot = duk_get_int(ctx, 1);

	//MessageBoxW(nullptr, weaponObject->GetFullName().c_str(), L"OK", 0);
	//MessageBoxW(nullptr, std::to_wstring(slot).c_str(), L"OK", 0);

	Inventory::AddItemToInventoryWithUpdate(weaponObject, EFortQuickBars::Primary, slot, 1);

	return 0;
}

//void USpawnPickupAtLocation(weaponObject, x, y, z);
static duk_ret_t duk_spawnpickupatlocation(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 4)
	{
		MessageBox(nullptr, L"This function takes 4 arguments!.", L"USpawnPickupAtLocation", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* object = (UObject*)duk_get_pointer(ctx, 0);

	if (!object || Util::IsBadReadPtr(object))
	{
		MessageBox(nullptr, L"Weapon object is null.", L"USpawnPickupAtLocation", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto x = static_cast<float>(duk_get_int(ctx, 1));
	auto y = static_cast<float>(duk_get_int(ctx, 2));
	auto z = static_cast<float>(duk_get_int(ctx, 3));

	Pickup::SpawnPickupAtLocation(object, 1, {x, y, z});

	return 0;
}

//int UGetActorLocation(actorPointer, index);
static duk_ret_t duk_getactorlocation(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"UGetActorLocation", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* actorObject = (UObject*)duk_get_pointer(ctx, 0);

	if (!actorObject || Util::IsBadReadPtr(actorObject))
	{
		MessageBox(nullptr, L"Object was not found.", L"UGetActorLocation", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto index = duk_get_int(ctx, 1);

	//TODO: return an array or json.
	auto location = AActor::GetLocation(actorObject);

	switch (index)
	{
	case 1: //X
		{
			duk_push_int(ctx, location.X);
			printf("Return X: %f\n", location.X);
			break;
		}
	case 2: //Y
		{
			duk_push_int(ctx, location.Y);
			printf("Return Y: %f\n", location.Y);
			break;
		}
	case 3: //Z
		{
			duk_push_int(ctx, location.Z);
			printf("Return Z: %f\n", location.Z);
			break;
		}
	default: duk_push_int(ctx, location.X);
	}

	return 1; //one return value
} //UNDOCUMENTED

//UObject* USpawnTextActor([X, Y, Z], [Pitch, Yaw, Roll]);
static duk_ret_t duk_spawntextactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USpawnTextActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto locationArraySize = duk_get_length(ctx, 0);

	auto rotationArraySize = duk_get_length(ctx, 1);

	if (rotationArraySize == 3 && locationArraySize == 3)
	{
		duk_get_prop_index(ctx, 0, 0);
		auto x = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 0, 1);
		auto y = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 0, 2);
		auto z = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 0);
		auto pitch = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 1);
		auto yaw = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 2);
		auto roll = duk_get_int(ctx, -1);

		FVector Location{x, y, z};
		FRotator Rotation{pitch, yaw, roll};

		auto actor = TextActor::Spawn(Location, Rotation);

		if (!actor || Util::IsBadReadPtr(actor))
		{
			MessageBox(nullptr, L"Failed to spawn actor.", L"USpawnTextActor", 0);
			return DUK_RET_TYPE_ERROR;
		}

		duk_push_pointer(ctx, actor);
	}
	else
	{
		MessageBox(nullptr, L"Location/Rotations is not correct.", L"USpawnTextActor", 0);
		return DUK_RET_TYPE_ERROR;
	}


	return 1;
}

//void USetTextActorText(actorPointer, "New text");
static duk_ret_t duk_settextactortext(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USetTextActorText", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* actorObject = (UObject*)duk_get_pointer(ctx, 0);

	if (!actorObject || Util::IsBadReadPtr(actorObject))
	{
		MessageBox(nullptr, L"Actor pointer is not valid.", L"USetTextActorText", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string text = duk_get_string(ctx, 1);

	std::wstring textW(text.begin(), text.end());

	TextActor::SetText(actorObject, textW.c_str());

	return 0;
}

//void UActivateAbility(abilityClass);
static duk_ret_t duk_activateability(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 1)
	{
		MessageBox(nullptr, L"This function takes 1 arguments!.", L"UActivateAbility", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* abilityClass = (UObject*)duk_get_pointer(ctx, 0);

	if (!abilityClass || Util::IsBadReadPtr(abilityClass))
	{
		MessageBox(nullptr, L"Ability class pointer is not valid.", L"UActivateAbility", 0);
		return DUK_RET_TYPE_ERROR;
	}

	Player::GrantGameplayAbility(Globals::Pawn, abilityClass);

	return 0;
}

//void URenderASCIIWithActor(objectPointer, ASCIIMap, lineLength, itemWidth, itemWidth, [X, Y, Z], [Pitch, Yaw, Roll]);
static duk_ret_t duk_renderasciiwithactor(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 6)
	{
		MessageBox(nullptr, L"This function takes 6 arguments!.", L"URenderASCIIWithActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto actor = (UObject*)duk_get_pointer(ctx, 0);

	if (!actor || Util::IsBadReadPtr(actor))
	{
		MessageBox(nullptr, L"Actor class is invalid.", L"URenderASCIIWithActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string map = duk_get_string(ctx, 1);

	int lineLength = duk_get_int(ctx, 2);

	float actorWidth = duk_get_int(ctx, 3);

	float actorHeight = duk_get_int(ctx, 4);

	auto locationArraySize = duk_get_length(ctx, 5);

	auto rotationArraySize = duk_get_length(ctx, 6);

	if (rotationArraySize == 3 && locationArraySize == 3)
	{
		duk_get_prop_index(ctx, 5, 0);
		auto x = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 5, 1);
		auto y = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 5, 2);
		auto z = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 6, 0);
		auto pitch = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 6, 1);
		auto yaw = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 6, 2);
		auto roll = duk_get_int(ctx, -1);

		FVector Location{x, y, z};
		FRotator Rotation{pitch, yaw, roll};

		//printf("%s", map.c_str());

		printf("x: %f, y: %f, z: %f\n", Location.X, Location.Y, Location.Y);

		Render::MapWithActor(actor, map, actorWidth, actorHeight, lineLength, Location, Rotation);
	}
	else
	{
		MessageBox(nullptr, L"Location/Rotations is not correct.", L"URenderASCIIWithActor", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 0;
}

//WebClientPointer UWebClient("Host");
static duk_ret_t duk_webclient(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 1)
	{
		MessageBox(nullptr, L"This function takes 1 arguments!.", L"UWebClient", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string host = duk_get_string(ctx, 0);

	if (!host.empty())
	{
		static httplib::SSLClient cli(host);

		duk_push_pointer(ctx, &cli);
	}
	else
	{
		MessageBox(nullptr, L"Host name cannot be empty!.", L"UWebClient", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 1; //one return value
}

//MISSING: A function to add headers, bodys etc etc.

//String UWebClientGet(Client, "Path");
static duk_ret_t duk_webclientget(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"UWebClientGet", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto cli = (httplib::SSLClient*)duk_get_pointer(ctx, 0);

	if (!cli || Util::IsBadReadPtr(cli))
	{
		MessageBox(nullptr, L"Client is invalid.", L"UWebClientGet", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string path = duk_get_string(ctx, 1);

	if (!path.empty())
	{
		if (auto res = (*cli).Get(path.c_str()))
		{
			if (res->status == 200)
			{
				duk_push_string(ctx, res->body.c_str());
			}
		}
	}
	else
	{
		MessageBox(nullptr, L"Path cannot be empty!.", L"UWebClientGet", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 1; //one return value
}

//String UWebClientPost(Client, "Path", "Body", "Content-Type");
static duk_ret_t duk_webclientpost(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 4)
	{
		MessageBox(nullptr, L"This function takes 4 arguments!.", L"UWebClientPost", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto cli = (httplib::SSLClient*)duk_get_pointer(ctx, 0);

	if (!cli || Util::IsBadReadPtr(cli))
	{
		MessageBox(nullptr, L"Client is invalid.", L"UWebClientPost", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string path = duk_get_string(ctx, 1);
	std::string body = duk_get_string(ctx, 2);
	std::string type = duk_get_string(ctx, 3);

	if (!path.empty())
	{
		if (auto res = (*cli).Post(path.c_str(), body.c_str(), type.c_str()))
		{
			if (res->status == 200)
			{
				duk_push_string(ctx, res->body.c_str());
			}
		}
	}
	else
	{
		MessageBox(nullptr, L"Path cannot be empty!.", L"UWebClientPost", 0);
		return DUK_RET_TYPE_ERROR;
	}

	return 1; //one return value
}

/*
//void UTriggerWin();
static duk_ret_t duk_triggerwin(duk_context* ctx)
{
	Player::TriggerWin();
	return 0;
}*/

/*
//void UProcessEventHook("EVENT", function() {});
static duk_ret_t duk_processeventhook(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength != 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"UProcessEventHook", 0);
		return DUK_RET_TYPE_ERROR;
	}

	std::string event = duk_get_string(ctx, 0);
	std::wstring eventW(event.begin(), event.end());

	duk_require_function(ctx, 1);
	duk_dup(ctx, 1);
	duk_put_global_string(ctx, event.c_str());

	Globals::ProcessEventHooks.push_back(eventW);

	return 0;
}*/

//void USpawnBot([X, Y, Z], [Pitch, Yaw, Roll]);
static duk_ret_t duk_spawnbot(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USpawnBot", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto locationArraySize = duk_get_length(ctx, 0);

	auto rotationArraySize = duk_get_length(ctx, 1);

	if (rotationArraySize == 3 && locationArraySize == 3)
	{
		duk_get_prop_index(ctx, 0, 0);
		auto x = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 0, 1);
		auto y = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 0, 2);
		auto z = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 0);
		auto pitch = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 1);
		auto yaw = duk_get_int(ctx, -1);

		duk_get_prop_index(ctx, 1, 2);
		auto roll = duk_get_int(ctx, -1);

		FVector Location{x, y, z};
		FRotator Rotation{pitch, yaw, roll};

		if (!Globals::BotController)
		{
			Globals::BotController = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.AthenaAIController"), FVector{0, 0, 10000}, {});

			Globals::BotPawn = SpawnActorEasy(GetWorld(), FindObject(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"), Location, Rotation);

			auto botPlayerState = SpawnActorEasy(GetWorld(), FindObject(L"Class /Script/FortniteGame.FortPlayerStateAthena"), FVector{0, 0, 2792}, {});

			if (Globals::BotPawn)
			{
				Player::Possess(Globals::BotController, Globals::BotPawn);
				Player::SetMaxHealth(Globals::BotPawn, 100);
				Player::SetHealth(Globals::BotPawn, 100);
			}
			else
			{
				MessageBox(nullptr, L"Couldn't spawn a bot pawn.", L"USpawnBot", 0);
				return DUK_RET_TYPE_ERROR;
			}

			auto PlayerStatePawn = reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::BotPawn) + Offsets::PlayerStatePawnOffset);
			auto PlayerStateController = reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::BotController) + Offsets::PlayerStateOffset);

			*PlayerStatePawn = botPlayerState;
			*PlayerStateController = botPlayerState;

			ProcessEvent(Globals::BotPawn, FindObject(L"Function /Script/Engine.Pawn.OnRep_PlayerState"), nullptr);
			ProcessEvent(Globals::BotController, FindObject(L"Function /Script/Engine.Controller.OnRep_PlayerState"), nullptr);

			auto sk = FindObject(L"SkeletalMesh /Game/Characters/Survivors/Female/Small/F_SML_Starter_01/Meshes/F_SML_Starter_Epic.F_SML_Starter_Epic");
			if (sk)
			{
				Player::SetSkeletalMesh(Globals::BotPawn, sk);
			}

			//TODO: figure out why char parts doesn't show
			/*
			UObject* HeadCharacterPart = FindObject(L"CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
			UObject* BodyCharacterPart = FindObject(L"CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

			Player::ServerChoosePart(botPawn, EFortCustomPartType::Body, BodyCharacterPart);
			Player::ServerChoosePart(botPawn, EFortCustomPartType::Head, HeadCharacterPart);

			ProcessEvent(botPlayerState, FindObject(L"Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterParts"), nullptr);
			*/

			duk_push_pointer(ctx, Globals::BotPawn);
		}
		else
		{
			MessageBox(nullptr, L"Location/Rotations is not correct.", L"USpawnBot", 0);
			return DUK_RET_TYPE_ERROR;
		}
	}

	return 1;
}

//void UMoveBotToTarget(playerPawnPointer, X, Y, Z);
static duk_ret_t duk_movebottotarget(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 4)
	{
		MessageBox(nullptr, L"This function takes 4 arguments!.", L"UMoveBotToTarget", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto pawn = (UObject*)duk_get_pointer(ctx, 0);

	if (!pawn || Util::IsBadReadPtr(pawn))
	{
		MessageBox(nullptr, L"Pawn poinwer is invalid.", L"UMoveBotToTarget", 0);
		return DUK_RET_TYPE_ERROR;
	}


	auto x = static_cast<float>(duk_get_int(ctx, 1));
	auto y = static_cast<float>(duk_get_int(ctx, 2));
	auto z = static_cast<float>(duk_get_int(ctx, 3));

	FVector Location{x, y, z};

	if (Globals::BotController)
	{
		Globals::BotTarget = Location;
	}

	return 0;
}

//void USetPlayerMaxHealth(playerPawnPointer, newMaxHealth);
static duk_ret_t duk_setplayermaxhealth(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USetPlayerMaxHealth", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* playerPawnPointer = (UObject*)duk_get_pointer(ctx, 0);

	if (!playerPawnPointer || Util::IsBadReadPtr(playerPawnPointer))
	{
		MessageBox(nullptr, L"Player pawn pointer is not valid.", L"USetPlayerMaxHealth", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto newMaxHealth = duk_get_int(ctx, 1);

	Player::SetMaxHealth(playerPawnPointer, newMaxHealth);

	return 0;
}

//void USetPlayerHealth(playerPawnPointer, newHealth);
static duk_ret_t duk_setplayerhealth(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USetPlayerHealth", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* playerPawnPointer = (UObject*)duk_get_pointer(ctx, 0);

	if (!playerPawnPointer || Util::IsBadReadPtr(playerPawnPointer))
	{
		MessageBox(nullptr, L"Player pawn pointer is not valid.", L"USetPlayerHealth", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto newHealth = duk_get_int(ctx, 1);

	Player::SetHealth(playerPawnPointer, newHealth);

	return 0;
}

//void USetPlayerMaxShield(playerPawnPointer, newHealth);
static duk_ret_t duk_setplayermaxshield(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USetPlayerMaxShield", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* playerPawnPointer = (UObject*)duk_get_pointer(ctx, 0);

	if (!playerPawnPointer || Util::IsBadReadPtr(playerPawnPointer))
	{
		MessageBox(nullptr, L"Player pawn pointer is not valid.", L"USetPlayerMaxShield", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto newHealth = duk_get_int(ctx, 1);

	Player::SetMaxShield(playerPawnPointer, newHealth);

	return 0;
}

//void USetPlayerShield(playerPawnPointer, newHealth);
static duk_ret_t duk_setplayershield(duk_context* ctx)
{
	int ArgsLength = duk_get_top(ctx);
	if (ArgsLength < 2)
	{
		MessageBox(nullptr, L"This function takes 2 arguments!.", L"USetPlayerShield", 0);
		return DUK_RET_TYPE_ERROR;
	}

	UObject* playerPawnPointer = (UObject*)duk_get_pointer(ctx, 0);

	if (!playerPawnPointer || Util::IsBadReadPtr(playerPawnPointer))
	{
		MessageBox(nullptr, L"Player pawn pointer is not valid.", L"USetPlayerShield", 0);
		return DUK_RET_TYPE_ERROR;
	}

	auto newHealth = duk_get_int(ctx, 1);

	Player::SetShield(playerPawnPointer, newHealth);

	return 0;
}
