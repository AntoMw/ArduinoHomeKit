#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <math.h>
//#include <avr/wdt.h>//watchdog

//Backbone
#include "env.h"
#include "typedefs.h"

//Modules
#include "HomeControl.h"
#include ""


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



HomeKit_st = Disconnected;

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
  QueryCityData(WeatherQStr , false);
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



