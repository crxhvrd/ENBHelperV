//------------------------------------------------------------------------------------------------
//ENBSeries for Grand Theft Auto V
//------------------------------------------------------------------------------------------------

#define HELPER_COMPILATION
#include "script.h"
#include <stdio.h>

bool bInitialized = false;

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


float	ENBTime = 12.0f;
float	ENBCameraPosition[3] = { 0.0f, 0.0f, 0.0f };
float	ENBWeatherTransition = 0.0f;
DWORD	ENBWeatherCurrent = 0;
DWORD	ENBWeatherOutgoing = 0;
float	ENBWindSpeed = 0.0f;
float	ENBWindDirection[3] = {0.0f, 0.0f, 0.0f };
float	ENBRainAmount = 0.0f;
float	ENBSnowAmount = 0.0f;
BOOL	ENBIsInterior = FALSE;

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


EEXPORT bool HELPER_API GetTime(float& time)
{
	time = ENBTime;
	return true;
}

EEXPORT bool HELPER_API GetWeatherTransition(float& t)
{
	t = ENBWeatherTransition;
	return true;
}

EEXPORT bool HELPER_API GetCurrentWeather(DWORD& id)
{
	id = ENBWeatherCurrent;
	return true;
}

EEXPORT bool HELPER_API GetOutgoingWeather(DWORD& id)
{
	id = ENBWeatherOutgoing;
	return true;
}

EEXPORT bool HELPER_API GetWindSpeed(float& s)
{
	s = ENBWindSpeed;
	return true;
}

EEXPORT bool HELPER_API GetWindDirection(float *d)
{
	if (!d) return false;
	//memcpy(d, ENBWindDirection, 3 * sizeof(float));
	d[0] = ENBWindDirection[0];
	d[1] = ENBWindDirection[1];
	d[2] = ENBWindDirection[2];
	return true;
}

EEXPORT bool HELPER_API GetRainAmount(float& s)
{
	s = ENBRainAmount;
	return true;
}

EEXPORT bool HELPER_API GetSnowAmount(float& s)
{
	s = ENBSnowAmount;
	return true;
}

EEXPORT bool HELPER_API IsInterior(BOOL& i)
{
	i = ENBIsInterior;
	return true;
}

EEXPORT bool HELPER_API GetCameraPosition(float *p)
{
	if (!p) return false;
	p[0] = ENBCameraPosition[0];
	p[1] = ENBCameraPosition[1];
	p[2] = ENBCameraPosition[2];
	return true;
}

/*
EEXPORT bool HELPER_API GetPlayerCameraTransformMatrices(NiTransform& m_local, NiTransform& m_world)
{
	DWORD playercamera = *(DWORD *)ADDR_PLAYER_CAMERA;
	if (!playercamera) return false;
	DWORD camnode = *(DWORD *)(playercamera + OFFSET_NODE);
	if (!camnode) return false;
	memcpy(&m_local, (NiTransform *)(camnode + OFFSET_LOCAL_MATRIX), sizeof(NiTransform));
	memcpy(&m_world, (NiTransform *)(camnode + OFFSET_WORLD_MATRIX), sizeof(NiTransform));
	return true;
}

EEXPORT bool HELPER_API GetCurrentLocationId(DWORD& id)
{
	DWORD player = *(DWORD *)ADDR_PLAYER;
	if (!player) return false;
	TGetDwNativeFunc f = (TGetDwNativeFunc)FUNC_GET_CURRENT_LOCATION;
	DWORD location = f(0, 0, player);
	if (!location) return false;
	id = getFormId(location);
	return true;
}

EEXPORT bool HELPER_API GetWorldSpaceId(DWORD& id)
{
	DWORD player = *(DWORD *)ADDR_PLAYER;
	if (!player) return false;
	TGetDwNativeFunc f = (TGetDwNativeFunc)FUNC_GET_WORLD_SPACE;
	DWORD worldSpace = f(0, 0, player);
	if (!worldSpace) return false;
	id = getFormId(worldSpace);
	return true;
}
*/

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


EEXPORT bool HELPER_API IsLoaded()
{
	//if (!bInitialized) return false;
	return true;
}


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
void	update()
{
	//+++ time
	{
		float	h = (float)(GET_CLOCK_HOURS());
		float	m = (float)(GET_CLOCK_MINUTES());
		float	s = (float)(GET_CLOCK_SECONDS());
		ENBTime = h + (m / 60.0f) + (s / (60.0f*60.0f));
		if (ENBTime < 0.0f) ENBTime = 0.0f;
		if (ENBTime >= 24.0f) ENBTime = 0.0f;
	}

	//+++ weather
	{
		DWORD	weathercurrent = GET_PREV_WEATHER_TYPE_HASH_NAME();
		DWORD	weathernext = GET_NEXT_WEATHER_TYPE_HASH_NAME();
		float	weathertransition = 0.0f;
	//	SET_CURR_WEATHER_STATE(weathercurrent, weathernext, 0.5f);
		GET_CURR_WEATHER_STATE(&weathercurrent, &weathernext, &weathertransition);
		ENBWeatherCurrent = weathernext;
		ENBWeatherOutgoing = weathercurrent;
		ENBWeatherTransition = weathertransition;
		if (ENBWeatherTransition < 0.0f) ENBWeatherTransition = 0.0f;
		if (ENBWeatherTransition > 1.0f) ENBWeatherTransition = 1.0f;
	}

	//+++ wind
	{
		float	windspeed = 0.0f;
		windspeed = GET_WIND_SPEED();
		ENBWindSpeed = windspeed;
		if (ENBWindSpeed < 0.0f) ENBWindSpeed = 0.0f;
		if (ENBWindSpeed > 10000.0f) ENBWindSpeed = 10000.0f;
		Vector3	winddir = GET_WIND_DIRECTION();
		ENBWindDirection[0] = winddir.x;
		ENBWindDirection[1] = winddir.y;
		ENBWindDirection[2] = winddir.z;
	}

	//+++ rain and snow
	{
		DWORD	rain = 0;
		DWORD	snow = 0;
		rain = GET_RAIN_LEVEL();
		memcpy(&ENBRainAmount, &rain, 4);
		if (ENBRainAmount < 0.0f) ENBRainAmount = 0.0f;
		if (ENBRainAmount > 1.0f) ENBRainAmount = 1.0f;
		snow = GET_SNOW_LEVEL();
		memcpy(&ENBSnowAmount, &snow, 4);
		if (ENBSnowAmount < 0.0f) ENBSnowAmount = 0.0f;
		if (ENBSnowAmount > 1.0f) ENBSnowAmount = 1.0f;
	}

	//+++ interior
	{
		//useless, just few places have this, Michael house don't have (but underground do have)
		ENBIsInterior = IS_INTERIOR_SCENE();
	}

	//+++ camera
	//Cam	camera = GET_RENDERING_CAM();
	//	Cam	camera = GET_RENDERING_CAM();
	//	if (TRUE == IS_GAMEPLAY_CAM_RENDERING())
	//	if (TRUE == DOES_CAM_EXIST(camera))
	{
		//	Vector3	cameraposition = GET_GAMEPLAY_CAM_COORD(); //not work at moment camera move from top to bottom at hospital spawn
		Vector3	cameraposition = GET_GAMEPLAY_CAM_COORDS();
		ENBCameraPosition[0] = cameraposition.x;
		ENBCameraPosition[1] = cameraposition.y;
		ENBCameraPosition[2] = cameraposition.z;
		//Vector3	camerarotation = GET_GAMEPLAY_CAM_ROT();
	}

	//light like this for transparent objects only, so it's useless
//	Vector3	lightpos = GET_GAMEPLAY_CAM_COORDS();
//	GRAPHICS::DRAW_SPOT_LIGHT_WITH_SHADOW(lightpos.x, lightpos.y, lightpos.z, 0.0f, 0.0f, 1.0f, 127, 255, 127, 10000.0f, 10.0f, 0.5f, 1000.0f, 0.0001f, 0.5f);
//	GRAPHICS::DRAW_LIGHT_WITH_RANGE_WITH_SHADOW(lightpos.x, lightpos.y, lightpos.z, 255, 64, 64, 100000.0f, 1000.0f, 1.0f);

	//zone is name of location shortened, like STRAW is Strawberry, KOREAT is korean town.
/*	Vector3	lightpos = GET_GAMEPLAY_CAM_COORDS();
	//int	zone = GET_ZONE_AT_COORDS(lightpos.x, lightpos.y, lightpos.z);
	char	*zonename = GET_NAME_OF_ZONE(lightpos.x, lightpos.y, lightpos.z);
	if (zonename)
	{
		FILE	*file = fopen("__temp.txt", "at");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			fprintf(file, zonename);
			fclose(file);
		}
	}*/
/*	Vector3	lightpos = GET_GAMEPLAY_CAM_COORDS();
	int	zone = GET_ZONE_AT_COORDS(lightpos.x, lightpos.y, lightpos.z);
	//Hash	zh = GET_HASH_OF_MAP_AREA_AT_COORDS(lightpos.x, lightpos.y, lightpos.z);
	{
		FILE	*file = fopen("__temp.txt", "at");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			//fprintf(file, "%08X \n", zh);
			fprintf(file, "%08X \n", zone);
			fclose(file);
		}
	}*/
	//don't work too for detecting interiors purely, far from precise
//	Vector3	lightpos = GET_GAMEPLAY_CAM_COORDS();
//	ENBIsInterior = GET_INTERIOR_AT_COORDS(lightpos.x, lightpos.y, lightpos.z);
}



//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

void	ScriptMain()
{
	srand(GetTickCount());
	while (true)
	{
		update();
		WAIT(0);
	}
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------