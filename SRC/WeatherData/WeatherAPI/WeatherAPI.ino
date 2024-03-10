#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <math.h>
//#include <avr/wdt.h>//watchdog

#include "env.h"
#include "typedefs.h"

//Macros
#define BAUDRATE 115200
#define FULL_GEO_STR "https://api.openweathermap.org/geo/1.0/direct?q=" BIRTH_CITY "," BIRTH_COUNTRY "&limit=1&appid=" APIKEY
#define FULL_WEATHER_STR "https://api.openweathermap.org/data/2.5/weather?lat=" HOME_LAT "lon=" HOME_LON "&units=metric&appid=" APIKEY
#define MAX_TMP_CNT 0xFF
#define TMP_CNT_INC_DEC_RATIO 0x5

//pins
#define STATUS_LED     2
#define CNT_LED        16
#define FETCH_LED      17   


//variables
CityQuery_st_t CityData;
Weather_st_t CityWeather;

HomeKit_st_t HomeKit_st = Disconnected;
HTTPClient client;
int httpCode =-1;


void setup() {
  // put your setup code here, to run once:
  //pinModes
  pinMode(STATUS_LED, OUTPUT);
  pinMode(CNT_LED, OUTPUT);
  pinMode(FETCH_LED, OUTPUT);


  Serial.begin(BAUDRATE);
  WiFi.begin(SSID, PWORD);
  
  Serial.println("\n++++++++++++++++\nHomekit\n++++++++++++++++");
  Serial.print("\nConnecting to WiFi");

  while(WL_CONNECTED != WiFi.status())
  {
    Serial.print("...");
    delay(500);
  }

  Serial.println("\nConnected to the WiFi Network");
  Serial.print("Ip Address: ");
  Serial.println(WiFi.localIP());
  HomeKit_st = Connected;

  Serial.println("HomeKit_st Val: "+String(HomeKit_st));

  //Serial.println(APIStr);
  //get coordinates for NBO
  QueryCityData(FULL_GEO_STR , false);
  //QueryCityData(FULL_GEO_STR , false);
  
  
}

void loop() 
{
  //variables
  int8_t tempQcnt =0;


  // put your main code here, to run repeatedly:
  if(WL_CONNECTED == WiFi.status())
  {
    HomeKit_st = Connected;
  }
  else if(WL_DISCONNECTED !=WiFi.status())
  {
    HomeKit_st = Disconnected;
  }
  else//error not defined
  {
    HomeKit_st = Undefined;
  }

  //state machine ==connected
  while(Connected == HomeKit_st)
  {
    
    //do stuff while kit is connected to internet
    delay(5000);
  }
  //loosing connection mid-execution
  while(Disconnected == HomeKit_st)
  {
    WiFi.begin(SSID, PWORD);
    Serial.print("\nWiFi Disconnected: Reconnecting to WiFi");

    while(WL_CONNECTED != WiFi.status())
    {
    Serial.print("...");
    delay(500);
    }
    HomeKit_st = Connected;
  }

  if(Undefined ==HomeKit_st)
  {
    //trigger reboot
    Serial.println("\n An Internal Error Occured, Rebooting the HomeKit...")
    for(int8_t i=0;i<9;i++)
    {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      delay(500);
    }
    delay(1000);
    //reboot();
  }

  delay(1000);//the loop function is delayed 1s‚
}

//reboots via watchdog
/*void reboot() 
{
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) { 
  }

  for(int8_t i=0;i<5;i++)
  { 
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED);
    delay(500);
  }
}

*/

//
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


