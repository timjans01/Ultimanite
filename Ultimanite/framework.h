#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include <Windows.h>
#include <vector>
#include <Detours.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>

#include <duktape.h>

//always the last
#include "consts.h"
#include "enums.h"
#include "util.h"
#include "structs.h"
#include "ue4.h"
#include "sdk.h"
#include "script.h"