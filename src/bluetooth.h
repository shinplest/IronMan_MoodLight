#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include <SoftwareSerial.h>
int Tx = 6; //전송
int Rx = 7; //수신
SoftwareSerial btSerial(Tx, Rx);

#endif