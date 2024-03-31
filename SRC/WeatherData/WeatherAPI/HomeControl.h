//Author    : Kioko M
//Date      : 31.03.2024
//Purpose   : Contains the declarations, type defintions for the Weather HomeKit App

#ifndef homecontrol_h
#define homecontrol_h

//denotes the operating state of the homekit
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

#endif



