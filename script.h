
#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include "..\enbhelper.h"

#ifndef __cplusplus
#define EEXPORT				_declspec(dllexport)
#else
#define EEXPORT	extern	"C"	_declspec(dllexport)
#endif

void ScriptMain();