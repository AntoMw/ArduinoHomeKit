//Author    : Kioko
//Purpose   : Holds all the type defintions for the Weather HomeKit App

#ifndef typedefs_h
#define typedefs_h

typedef enum 
{
  Disconnected =0xFF,
  Connected = 0x0,    //0x00
  InternalError,
  InputError,
  Undefined,
  ConnectionFailed,
  ConnectionLost
  
} HomeKit_st_t; 


typedef struct
{
  String Name;
  float Lat;
  float Lon;
} CityQuery_st_t;



typedef struct
{
  String Name;
  String Country;
  float Temp;//current.temp
  float TempMax;//daily.temp.max
  float TempMin;//daily.temp.min
  float FeelsLike;//current.feels_like
  float  UVidx;//hourly.uvi
  String HH_MainWeather;//hourly.weather.main
  String HH_WeatherDesc;//hourly.weather.description
  String DD_DailySummary;//daily.summary
}Weather_st_t;

#endif



