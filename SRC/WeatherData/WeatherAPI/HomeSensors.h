//Author    : Kioko M
//Date      : 16.03.2024
//Purpose   : Contains the declarations for the Weather Query 
//            functionality of the HomeKit


#ifndef homesensors_h
#define homesensors_h

//sensors, PINs
#define L_TEMP_SENSOR 15
#define B_TEMP_SENSOR 16
#define L_LIGHT_SENSOR 17
#define B_LIGHT_SENSOR 18
#define L_HUMIDITY_SENSOR 19

//outputs


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



//functions
extern float getCurrTemp(float adcCNT, int maxR, float VCC );
extern float getLightIntensity(int Room, int Window);
extern float getHumidtyLevel(int Room);
extern int getSwitchStatus(int SwitchNo);
extern int setSwitchState(int SwitchNo, int State);
extern void toggleSwitchState(int SwitchNo);

#endif



