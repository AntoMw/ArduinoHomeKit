#include <stdio.h>
#include <math.h>
#include "HomeSensors.h"

//macros
#define MAX_TMP_CNT 0xFF
#define LIMIT(X, Min, Max) ((x)<(Min)?(Min):((x)>(Max)?(Max):(x)))
#define TMP_CNT_INC_DEC_RATIO 0x5

/*/ Steinhart-Hart coefficients for the NTC thermistor
#define A 0.001129148
#define B 0.000234125
#define C 0.0000000876741*/

#define A = 1.009249522e-03
#define B = 2.378405444e-04
#define C = 2.019202697e-07

//get temperature from NTC 10k
float getCurrTemp(float adcCNT, int maxR, float VCC )
{
    float tmpTemp = -1;
    float V = -1;
    float R =-1;

    if(20> adcCNT)
    {
        tmpTemp =0xFE; // Sensor Err, Short to GND
    }
    else if(1020<adcCNT)
    {
        tmpTemp =0xFF; // Sensor Err , Short to VCC
    }
    else
    {
        V = adcCNT*(VCC/1023.0);
        R = maxR*(VCC/V-1.0);

        tmpTemp = 1.0 / (A + B * log(R) + C * pow(log(R), 3));

        if(-40>tmpTemp || 60<tmpTemp)
        {
            tmpTemp =0xFD; // temp Val NPL
        }
    }
    return tmpTemp;
}

float getLightIntensity(int Room, int Window);
float getHumidtyLevel(int Room);
int getSwitchStatus(int SwitchNo);
int setSwitchState(int SwitchNo, int State);
void toggleSwitchState(int SwitchNo);