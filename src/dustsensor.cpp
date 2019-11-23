#include <Arduino.h>

extern int sharpLEDPin;
extern int sharpVoPin;
extern unsigned long VoRawTotal;
extern int VoRawCount;
extern float Voc;
extern float K;
extern int VoRaw;
extern float Vo;
extern float CurrentDust;
extern int AvgDustNum;



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
    while (VoRawCount <= AvgDustNum)
    {
        digitalWrite(sharpLEDPin, LOW);
        delayMicroseconds(280);
        VoRaw = analogRead(sharpVoPin);
        digitalWrite(sharpLEDPin, HIGH);
        delayMicroseconds(9620);
        VoRawTotal += VoRaw;
        VoRawCount++;
    }
    Vo = 1.0 * VoRawTotal / AvgDustNum;
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
