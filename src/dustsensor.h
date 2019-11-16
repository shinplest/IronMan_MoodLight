#ifndef __DUSTSENSOR_H__
#define __DUSTSENSOR_H__

#include "Arduino.h"

#define N 100

const int sharpLEDPin = 8;  
const int sharpVoPin = A1;  
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
static float Voc = 0.6;
const float K = 0.5;

void printValue(String text, unsigned int value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}
void printFValue(String text, float value, String units, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  Serial.print(units);
  if (!isLast) {
    Serial.print(", ");
  }
}

#endif