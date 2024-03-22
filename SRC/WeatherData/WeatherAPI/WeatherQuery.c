//Author    : Kioko
//Date      : 16.03.2024
//Purpose   : Contains the implementations for the Weather Query 
//            functionality of the HomeKit

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "typedefs.h"
#include "env.h"

#include "WeatherQuery.h"

//definitions go here...
void QueryCityData(String APIStr, bool isWeatherQ)
{
  String tmpCity = "NOT Supplied";
  String tmpC_Code = "NOT Supplied";
  string W_APIStr;
  client.begin(APIStr);
  httpCode =client.GET();

  if(0<httpCode)
  {
    String Res =client.getString();
    Serial.println("\nQuery StatusCode: "+String(httpCode));

    JsonDocument doc1;
    DeserializationError Err = deserializeJson(doc1, Res);
    
    if(Err)//bool
    {
      Serial.print("\nJson Deserialization Failed with Code: ");
      Serial.println(Err.f_str());//print error
      HomeKit_st = InternalError;
    }
    else
    {
      Serial.println("\nJson Deserialization Succeeded\n");
      if(isWeatherQ)//getting the weather Data
      {
        CityWeather.Name           = CityData.Name;
        CityWeather.Country        = CityData.Country;
        CityWeather.Temp           = doc1["current"]["temp"];
        CityWeather.TempMax        = doc1["daily"]["temp"]["max"];
        CityWeather.TempMin        = doc1["daily"]["temp"]["min"];
        CityWeather.FeelsLike      = doc1["current"]["feels_like"];
        CityWeather.UVidx          = doc1["hourly"]["uvi"];
        CityWeather.HH_MainWeather = doc1["hourly"]["weather"]["main"];
        CityWeather.HH_WeatherDesc = doc1["hourly"]["weather"]["description"];
        CityWeather.DD_DailySummary= doc1["daily"]["summary"];

        Serial.println("City Weather Querried: "+CityWeather.Name+", Country: "+CityWeather.Country+" & Daily Summary: ["+CityWeather.DD_DailySummary+"]\n");
      }
      else //querrying lat, lon
      {
        CityData.Name = doc1[0]["name"];
        CityData.Country = doc1[0]["country"];
        CityData.Lat = doc1[0]["lat"];
        CityData.Lon = doc1[0]["lon"];

        Serial.println("Querried City: "+CityData.Name+", Lat: "+CityData.Lat+" & Lon: "+CityData.Lon+"\n");
      }
      //Serial.println(Res);
    }

  }
  else
  {
    Serial.println("\n\n An Error Occured!, Status Code: "+String(httpCode));
  }

}
