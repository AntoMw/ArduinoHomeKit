//Author    : Kioko M
//Date      : 16.03.2024
//Purpose   : Implementation for the HomeControl Kit

#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <math.h>
//#include <avr/wdt.h>//watchdog

//Backbone
#include "env.h"
#include "HomeControl.h"
#include "HomeSensors.h"

//Main Macros
#define BAUDRATE 115200
#define FULL_GEO_STR "https://api.openweathermap.org/geo/1.0/direct?q=" BIRTH_CITY "," BIRTH_COUNTRY "&limit=1&appid=" APIKEY
#define FULL_WEATHER_STR "https://api.openweathermap.org/data/2.5/weather?lat=" HOME_LAT "lon=" HOME_LON "&units=metric&appid=" APIKEY
#define MAX_TMP_CNT 0xFF
#define TMP_CNT_INC_DEC_RATIO 0x5

//pins
#define STATUS_LED     2
#define CNT_LED        16
#define FETCH_LED      17   


//holds the City Query Data, use for unkown lat lon coordniates
typedef struct
{
  int Code;
  String Message;
  String Params;
} APIErrRes_st_t;

typedef struct
{
  String Name;
  String Country;
  float Lat;
  float Lon;
} CityQuery_st_t;

//struct for all needed weather details
typedef struct
{
  String Name;
  String Country;//Country Code
  float Temp;//current.temp
  float TempMax;//daily.temp.max
  float TempMin;//daily.temp.min
  float FeelsLike;//current.feels_like
  float  UVidx;//hourly.uvi
  String HH_MainWeather;//hourly.weather.main
  String HH_WeatherDesc;//hourly.weather.description
  String DD_DailySummary;//daily.summary
}Weather_st_t;

//function prototypes
void reboot();
void QueryCityData(String APIStr, int isWeatherQ);
void getWeatherD(int isWeatherQ);
HomeKit_st_t getApiReqErr(int HTTPRes);
float getCurrTemp(int Room);
float getLightIntensity(int Room, int Window);
float getHumidtyLevel(int Room);
int getSwitchStatus(int SwitchNo);
int setSwitchState(int SwitchNo, int State);
void toggleSwitchState(int SwitchNo);

//variables
APIErrRes_st_t APIErr;
CityQuery_st_t CityData;
Weather_st_t CityWeather;
HomeKit_st_t HomeKit_st;

HTTPClient client;
JsonDocument doc1;
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
  QueryCityData(WeatherQStr , 1);
  //QueryCityData(FULL_GEO_STR , false);
  
  
}

void loop() 
{
  int8_t tempQcnt =0;
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
    
    //delay(5000);
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
    Serial.println("\nAn Internal Error Occured, Rebooting the HomeKit...");
    int8_t i=0;
    for(i;i<9;i++)
    {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      delay(500);
    }
    delay(1000);
    //reboot();
  }
  digitalWrite(FETCH_LED, !digitalRead(FETCH_LED));
  delay(1000);//the loop function is delayed 1s‚
}

//definitions go here...
void QueryCityData(String APIStr, int isWeatherQ)
{
  String tmpCity = "NOT Suplied";
  String tmpC_Code = "NOT Suplied";
  String W_APIStr;
  //client.begin(APIStr);
  client.begin(FULL_WEATHER_STR);
  httpCode =client.GET();
  Serial.println("\nQuery: "+APIStr);

  if(0<httpCode)
  {
    String Res =client.getString();
    Serial.println("\nRes: "+Res);
    DeserializationError Err = deserializeJson(doc1, Res);
    
    if(Err)//bool
    {
      Serial.print("Json Deserialization Failed with Code: ");
      Serial.println(Err.f_str());//print error
      HomeKit_st = InternalError;
    }
    else
    {
      Serial.println("Json Deserialization Succeeded\n");
      //control logic for correct responses.
      switch(httpCode)
      {
        case 200:
          Serial.println("API Response: "+String(httpCode)+"\nMessage: Success");
          getWeatherD(isWeatherQ);
          break;
        default:
          getApiReqErr(httpCode);//APIresponse Err
          break;
      }
    }

  }
  else
  {
    Serial.println("\n\n An Error Occured!, Status Code: "+String(httpCode));
    HomeKit_st = InternalError;
  }

}

void getWeatherD(int isWeatherQ)
{
  if(isWeatherQ)//getting the weather Data
  {
    CityWeather.Name           = CityData.Name;
    CityWeather.Country        = CityData.Country;
    CityWeather.Temp           = doc1["current"]["temp"];
    CityWeather.TempMax        = doc1["daily"]["temp"]["max"];
    CityWeather.TempMin        = doc1["daily"]["temp"]["min"];
    CityWeather.FeelsLike      = doc1["current"]["feels_like"];
    CityWeather.UVidx          = doc1["hourly"]["uvi"];
    CityWeather.HH_MainWeather = (const char*)doc1["hourly"]["weather"]["main"];
    CityWeather.HH_WeatherDesc = (const char*)doc1["hourly"]["weather"]["description"];
    CityWeather.DD_DailySummary= (const char*)doc1["daily"]["summary"];

    Serial.println("City Weather Querried: "+CityWeather.Name+", Country: "+CityWeather.Country);
    Serial.println("Daily Summary: ["+CityWeather.DD_DailySummary+"]\n");

  }
  else //querrying lat, lon
  {
    CityData.Name    = (const char*)doc1[0]["name"];
    CityData.Country = (const char*)doc1[0]["country"];
    CityData.Lat     = doc1[0]["lat"];
    CityData.Lon     = doc1[0]["lon"];

    Serial.println("Querried City: "+CityData.Name+", Country: "+CityData.Country+"Lat: "+CityData.Lat+" & Lon: "+CityData.Lon+"\n");
  }
  //Serial.println(Res);
}

HomeKit_st_t getApiReqErr(int HTTPRes)
{
  APIErr.Code     = doc1["cod"];
  APIErr.Message  = (const char*)doc1["message"];
  APIErr.Params   = (const char*)doc1["parameters"];
  //control logic for possible weather API errorCodes
  Serial.println("API Response Error: "+String(HTTPRes));
  switch (HTTPRes)
  {
  case 400:
    Serial.println("Message: "+APIErr.Message+"\nReasons: Some mandatory parameters in the request are missing\n");
    break;
  case 401:
    Serial.println("Message: "+APIErr.Message+"\nReasons: API API token provided in the request does not grant access to this API.\n");
    break;
  case 404:
    Serial.println("Message: "+APIErr.Message+"\nReasons: There is no data for the entry you provided in the API request.\n");
    break;
  case 429:
    Serial.println("Message: "+APIErr.Message+"\nReasons: API Key quota of requests is exceeded.\n");
    break;
  default:
    Serial.println("Message: "+APIErr.Message+"\nReasons: The API Error Response is unique, please contact WeatherApi.org.\n");
    break;
  }

  HomeKit_st = InputError;
  return InputError;
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


