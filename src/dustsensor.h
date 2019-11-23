#ifndef __DUSTSENSOR_H__
#define __DUSTSENSOR_H__

#include "Arduino.h"

void printValue(String text, unsigned int value, bool isLast = false);
void printFValue(String text, float value, String units, bool isLast = false);
float CalculatDust();

#endif