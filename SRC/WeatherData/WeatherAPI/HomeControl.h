//Author    : Kioko
//Date      : 16.03.2024
//Purpose   : Contains the declarations for the Weather Query 
//            functionality of the HomeKit

#include "typedefs.h"

#ifndef homecontrol_h
#define homecontrol_h

//denotes the operating state of thze homekit
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

extern HomeKit_st_t HomeKit_st;


#endif



