//Author    : Kioko
//Date      : 16.03.2024
//Purpose   : Contains the declarations for the Weather Query 
//            functionality of the HomeKit

#include "typedefs.h"

#ifndef homesensors_h
#define homesensors_h

//sensors, PINs
#define L_TEMP_SENSOR 15
#define B_TEMP_SENSOR 16
#define L_LIGHT_SENSOR 17
#define B_LIGHT_SENSOR 18
#define L_HUMIDITY_SENSOR 19

//outputs




//functions
float getCurrTemp(int Room);
float getLightIntensity(int Room, int Window);
float getHumidtyLevel(int Room);
int8_t getSwitchStatus(int SwitchNo);
int8_t setSwitchState(int SwitchNo, int State);
void toggleSwitchState(int SwitchNo);

//rooms
typedef enum
{
  Hallway =0x0,
  Living,
  Kitchen,
  BedRoom1,
  Bedroom2,
  BathRoom,
  Storage,
  Balcony

}Rooms_en_t;

//normal 0/1 switches
typedef enum
{
  HallwaySw =0x0,
  LivingSw1,
  LivingSw2,
  BedRoom1Sw1,
  Bedroom2Sw2,
  BathRoomSw,
  StorageSw,
  BalconySw,

}Switches_en_t;

//blinds, electrical switches that can reverse blinds direction
typedef enum
{
  LivingRoom_B = 0x0,
  BedRoom1_B,
  BathRoom_B,
  Kitchen_B,
}Blinds_en_t;

//public variables
extern float CurrRoomTemp;
extern float CurrLightIntensity;





#endif



