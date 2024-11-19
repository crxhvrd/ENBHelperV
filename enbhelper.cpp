//------------------------------------------------------------------------------------------------
//ENBSeries for Grand Theft Auto V
//------------------------------------------------------------------------------------------------

#define HELPER_COMPILATION
#include "enbhelper.h"
#include "script.h"
#include <stdio.h>

bool bInitialized = false;

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


float	g_Time = 12.0f;
float	g_CameraPosition[3] = { 0.0f, 0.0f, 0.0f };
float	g_WeatherTransition = 0.0f;
DWORD   g_WeatherCurrent = 0;
DWORD   g_WeatherOutgoing = 0;
float	g_WindSpeed = 0.0f;
float	g_WindDirection[3] = {0.0f, 0.0f, 0.0f };
float	g_RainAmount = 0.0f;
float	g_SnowAmount = 0.0f;
BOOL	g_IsInterior = FALSE;

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


EEXPORT bool HELPER_API GetTime(float& time)
{
	time = g_Time;
	return true;
}

EEXPORT bool HELPER_API GetWeatherTransition(float& t)
{
	t = g_WeatherTransition;
	return true;
}

EEXPORT bool HELPER_API GetCurrentWeather(DWORD& id)
{
	id = g_WeatherCurrent;
	return true;
}

EEXPORT bool HELPER_API GetOutgoingWeather(DWORD& id)
{
	id = g_WeatherOutgoing;
	return true;
}

EEXPORT bool HELPER_API GetWindSpeed(float& s)
{
	s = g_WindSpeed;
	return true;
}

EEXPORT bool HELPER_API GetWindDirection(float *d)
{
	if (!d) return false;
	//memcpy(d, g_WindDirection, 3 * sizeof(float));
	d[0] = g_WindDirection[0];
	d[1] = g_WindDirection[1];
	d[2] = g_WindDirection[2];
	return true;
}

EEXPORT bool HELPER_API GetRainAmount(float& s)
{
	s = g_RainAmount;
	return true;
}

EEXPORT bool HELPER_API GetSnowAmount(float& s)
{
	s = g_SnowAmount;
	return true;
}

EEXPORT bool HELPER_API IsInterior(BOOL& i)
{
	i = g_IsInterior;
	return true;
}

EEXPORT bool HELPER_API GetCameraPosition(float *p)
{
	if (!p) return false;
	p[0] = g_CameraPosition[0];
	p[1] = g_CameraPosition[1];
	p[2] = g_CameraPosition[2];
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
		float h = static_cast<float>(CLOCK::GET_CLOCK_HOURS());
                float m = static_cast<float>(CLOCK::GET_CLOCK_MINUTES());
                float s = static_cast<float>(CLOCK::GET_CLOCK_SECONDS());
		g_Time = h + (m / 60.0f) + (s / (60.0f*60.0f));
		if (g_Time < 0.0f) g_Time = 0.0f;
		if (g_Time >= 24.0f) g_Time = 0.0f;
	}

	//+++ weather
	{
		DWORD weathercurrent = MISC::GET_PREV_WEATHER_TYPE_HASH_NAME();
                DWORD weathernext = MISC::GET_NEXT_WEATHER_TYPE_HASH_NAME();
		float	weathertransition = 0.0f;
	//	SET_CURR_WEATHER_STATE(weathercurrent, weathernext, 0.5f);
		MISC::GET_CURR_WEATHER_STATE(&weathercurrent, &weathernext, &weathertransition);
		g_WeatherCurrent = weathernext;
		g_WeatherOutgoing = weathercurrent;
		g_WeatherTransition = weathertransition;
		if (g_WeatherTransition < 0.0f) g_WeatherTransition = 0.0f;
		if (g_WeatherTransition > 1.0f) g_WeatherTransition = 1.0f;
	}

	//+++ wind
        {
                float windspeed = MISC::GET_WIND_SPEED();
                g_WindSpeed = windspeed;
                if (g_WindSpeed < 0.0f) g_WindSpeed = 0.0f;
                if (g_WindSpeed > 10000.0f) g_WindSpeed = 10000.0f;
                Vector3 winddir = MISC::GET_WIND_DIRECTION();
                g_WindDirection[0] = winddir.x;
                g_WindDirection[1] = winddir.y;
                g_WindDirection[2] = winddir.z;
        }


	//+++ rain and snow
	{
              float rain = MISC::GET_RAIN_LEVEL();
              g_RainAmount = rain;
              if (g_RainAmount < 0.0f) g_RainAmount = 0.0f;
              if (g_RainAmount > 1.0f) g_RainAmount = 1.0f;

              float snow = MISC::GET_SNOW_LEVEL();
              g_SnowAmount = snow;
              if (g_SnowAmount < 0.0f) g_SnowAmount = 0.0f;
              if (g_SnowAmount > 1.0f) g_SnowAmount = 1.0f;
        }

	//+++ interior
	{
		//useless, just few places have this, Michael house don't have (but underground do have)
		g_IsInterior = INTERIOR::IS_INTERIOR_SCENE();
	}

	//+++ camera
	//Cam	camera = GET_RENDERING_CAM();
	//	Cam	camera = GET_RENDERING_CAM();
	//	if (TRUE == IS_GAMEPLAY_CAM_RENDERING())
	//	if (TRUE == DOES_CAM_EXIST(camera))
	{
        Vector3 cameraposition = CAM::GET_GAMEPLAY_CAM_COORD();
        g_CameraPosition[0] = cameraposition.x;
        g_CameraPosition[1] = cameraposition.y;
        g_CameraPosition[2] = cameraposition.z;
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
//	g_IsInterior = GET_INTERIOR_AT_COORDS(lightpos.x, lightpos.y, lightpos.z);
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
