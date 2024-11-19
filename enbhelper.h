#pragma once

#include <windows.h>

#define HELPER_VER 1
#define HELPER_API __stdcall
#define HELPER_DLL "enbhelper.dll"

struct HelperFuncs
{
	DWORD version;
	// version = 1
	bool (HELPER_API *getTime)(float& time);
	bool (HELPER_API *getWeatherTransition)(float& t);
	bool (HELPER_API *getCurrentWeatherId)(DWORD& id);
	bool (HELPER_API *getOutgoingWeatherId)(DWORD& id);
	bool (HELPER_API *getWindSpeed)(float& s);
	bool (HELPER_API *getWindDirection)(float *d); //float3
	bool (HELPER_API *getRainAmount)(float& s);
	bool (HELPER_API *getSnowAmount)(float& s);
	bool (HELPER_API *isInterior)(BOOL& i);
	bool (HELPER_API *getCameraPosition)(float *p); //float3
/*	// version = 2
	bool (HELPER_API *getCurrentLocationId)(DWORD& id);
	bool (HELPER_API *getWorldSpaceId)(DWORD& id);*/
};

#ifdef HELPER_COMPILATION

bool HELPER_API helperInit(HelperFuncs& funcs);
bool HELPER_API helperUninit();

#else

// define in your cpp
// bool (HELPER_API *helperInit)(HelperFuncs& funcs) = NULL;
// bool (HELPER_API *helperUninit)() = NULL;
// HelperFuncs g_HelperFuncs;
// HMODULE g_HelperModule = NULL;

// first call helperLoad(), if succeed call helperInit() to check game version and get the funcs

//extern bool (HELPER_API *helperInit)(HelperFuncs& funcs);
//extern bool (HELPER_API *helperUninit)();
extern HelperFuncs	g_HelperFuncs;
extern HMODULE g_HelperModule;

__forceinline bool HELPER_API helperInit(HelperFuncs& funcs)
{
	funcs.version = HELPER_VER;
	//version 1
	funcs.getTime = (bool (HELPER_API *)(float& time))GetProcAddress(g_HelperModule, "GetTime");
	funcs.getWeatherTransition = (bool (HELPER_API *)(float& t))GetProcAddress(g_HelperModule, "GetWeatherTransition");
	funcs.getCurrentWeatherId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetCurrentWeather");
	funcs.getOutgoingWeatherId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetOutgoingWeather");
	funcs.getWindSpeed = (bool (HELPER_API *)(float& s))GetProcAddress(g_HelperModule, "GetWindSpeed");
	funcs.getWindDirection = (bool (HELPER_API *)(float *d))GetProcAddress(g_HelperModule, "GetWindDirection");
	funcs.getRainAmount = (bool (HELPER_API *)(float& s))GetProcAddress(g_HelperModule, "GetRainAmount");
	funcs.getSnowAmount = (bool (HELPER_API *)(float& s))GetProcAddress(g_HelperModule, "GetSnowAmount");
	funcs.isInterior = (bool (HELPER_API *)(BOOL& i))GetProcAddress(g_HelperModule, "IsInterior");
	funcs.getCameraPosition = (bool (HELPER_API *)(float *p))GetProcAddress(g_HelperModule, "GetCameraPosition");

/*	funcs.getCurrentLocationId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetCurrentLocationID");
	funcs.getWorldSpaceId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetWorldSpaceID");
*/
	return true;
}

__forceinline bool HELPER_API helperUninit()
{
	return true;
}

bool helperLoad(char *fullpath, char *filename);
void helperUnload();

#endif