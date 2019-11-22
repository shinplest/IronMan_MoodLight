#ifndef __DUSTSENSOR_H__
#define __DUSTSENSOR_H__

#include "Arduino.h"

#define N 10

const int sharpLEDPin = 8;
const int sharpVoPin = A1;
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
static float Voc = 0.6;
const float K = 0.5;
int VoRaw;
float Vo;
float CurrentDust = 0;

void printValue(String text, unsigned int value, bool isLast = false)
{
    Serial.print(text);
    Serial.print("=");
    Serial.print(value);
    if (!isLast)
    {
        Serial.print(", ");
    }
}
void printFValue(String text, float value, String units, bool isLast = false)
{
    Serial.print(text);
    Serial.print("=");
    Serial.print(value);
    Serial.print(units);
    if (!isLast)
    {
        Serial.print(", ");
    }
}

float CalculatDust()
{
    while (VoRawCount <= N)
    {
        digitalWrite(sharpLEDPin, LOW);
        delayMicroseconds(280);
        VoRaw = analogRead(sharpVoPin);
        digitalWrite(sharpLEDPin, HIGH);
        delayMicroseconds(9620);
        VoRawTotal += VoRaw;
        VoRawCount++;
    }
    Vo = 1.0 * VoRawTotal / N;
    VoRawCount = 0;
    VoRawTotal = 0;

    // Compute the output voltage in Volts.
    Vo = Vo / 1024.0 * 5.0;
    printFValue("Vo", Vo * 1000.0, "mV");

    // Convert to Dust Density in units of ug/m3.
    float dV = Vo - Voc;
    if (dV < 0)
    {
        dV = 0;
        Voc = Vo;
    }
    float dustDensity = dV / K * 100.0;
    printFValue("DustDensity", dustDensity, "ug/m3", true);
    Serial.println("");
    float dust = (int)(dustDensity - 70);
    if (dust < 0)
    dust = 0;
    return dust;
}

#endif