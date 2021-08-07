#pragma once

#define JM_XORSTR_DISABLE_AVX_INTRINSICS //OLD CPU SUPPORT

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

#include "source/duktape.h"
#include "xorstr.hpp"
#define _(str) xorstr_(str)

//always the last
#include "enums.h"
#include "util.h"
#include "structs.h"
#include "ue4.h"
#include "sdk.h"
#include "script.h"