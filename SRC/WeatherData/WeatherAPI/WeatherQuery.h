//Author    : Kioko
//Date      : 16.03.2024
//Purpose   : Contains the declarations for the Weather Query 
//            functionality of the HomeKit

#include "typedefs.h"

#ifndef weatherquery_h
#define weatherquery_h

//holds the City Query Data, use for unkown lat lon coordniates
typedef struct
{
  string Name;
  string Country;
  float Lat;
  float Lon;
} CityQuery_st_t;


//struct for all needed weather details
typedef struct
{
  string Name;
  string Country;//Country Code
  float Temp;//current.temp
  float TempMax;//daily.temp.max
  float TempMin;//daily.temp.min
  float FeelsLike;//current.feels_like
  float  UVidx;//hourly.uvi
  string HH_MainWeather;//hourly.weather.main
  string HH_WeatherDesc;//hourly.weather.description
  string DD_DailySummary;//daily.summary
}Weather_st_t;


void QueryCityData(string APIStr, bool isWeatherQ);

//variables
extern CityQuery_st_t CityData;
extern Weather_st_t CityWeather;



#endif



