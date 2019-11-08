#ifndef __NEOPIXEL_H__
#define __NEOPIXEL_H__

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 5       // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24 // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)

boolean LightState = false;        //전원 여부 판단
int ColorState = 1;                  //0 = red, 1 = white, 2 = green , 3 = blue
int brightness = 10;                 //네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


#endif