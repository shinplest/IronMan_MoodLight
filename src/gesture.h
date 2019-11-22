#ifndef __GESTURE_H__
#define __GESTURE_H__

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#define APDS9960_INT 2 // 제스쳐 인터럽트하는 출력 핀
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void interruptRoutine();

#endif