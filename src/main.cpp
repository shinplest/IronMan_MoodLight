#include <Arduino.h>
#include "bluetooth.h"
#include "gesture.h"
//블루투스 관련 선언
String data = "";
void getbtstring();
void bluetoothonoff();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//제스쳐관련 선언
int isr_flag = 0;
void interruptRoutine();
void handleGesture();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//네오픽셀관련 선언
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 5       // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24 // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)

int i;                          //네오픽셀 변수
boolean LightOn = false;        //전원 여부 판단
int r = 0, g = 0, b = 0, w = 0; //색 밝기 저장
int stage = 1;                  //0 = red, 1 = white, 2 = green , 3 = blue
int brightness = 10;            // 네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void colorWipe(uint32_t c, uint8_t wait);
void ChangeColor();
void TurnOnLight();
void TurnOffLight();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LCD관련 선언

#include <LiquidCrystal_I2C.h>      //LiquidCrystal 라이브러리 추가
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd 객체 선언

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//온도센서 관련 선언
int tempsensor = A0;

float getTemperature();
void printTemperature(float celsiustemp);


void btserialFlush(){
  while(btSerial.available() > 0) {
    char t = btSerial.read();
  }
} 


///
void turnbacklight(){
  lcd.backlight();     // 백라이트 켜기
  delay(2000);
  lcd.noBacklight(); // turn off backlight
}




















void setup()
{
  //////////////////////////////////////////////////////////////////////
  btSerial.begin(9600);
  Serial.begin(9600);
  //////////////////////////////////////////////////////////////////////
  lcd.init(); // LCD 초기화
  // Print a message to the LCD.
 
  lcd.setCursor(0, 0); // 1번째, 1라인
  lcd.print("2019-10-17 01:18");
  lcd.setCursor(0, 1); // 1번째, 2라인
  lcd.print("MISE MUNG:200ppm");

  turnbacklight();
  
  //////////////////////////////////////////////////////////////////////
  strip.setBrightness(brightness);
  strip.begin(); // 네오픽셀 제어 시작
  strip.show();  // 네오픽셀 점등
  //////////////////////////////////////////////////////////////////////
  pinMode(APDS9960_INT, INPUT);
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true))
  {
    Serial.println(F("Gesture sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}










void loop()
{
  if (isr_flag == 1)
  {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  getbtstring();
  bluetoothonoff();
 
}




























//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//네오픽셀 함수

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void ChangeColor()
{
  if (LightOn == true)
  {
    switch (stage)
    {
    case 0:
      colorWipe(strip.Color(255, 0, 0, 0), 30);
      Serial.println("빨간색으로 바꿉니다.");
      break;
    case 1:
      colorWipe(strip.Color(0, 0, 0, 255), 30);
      Serial.println("흰색으로 바꿉니다.");
      break;
    case 2:
      colorWipe(strip.Color(0, 255, 0, 0), 30);
      Serial.println("초록색으로 바꿉니다.");
      break;
    case 3:
      colorWipe(strip.Color(0, 0, 255, 0), 30);
      Serial.println("파란색으로 바꿉니다.");
      break;

    default:
      break;
    }
  }
}

void TurnOnLight()
{
   if (LightOn == false) //꺼져있을때만 점등
      {
        strip.begin();
        for (i = 0; i < 24; i++)
        {
          strip.setPixelColor(i, 0, 0, 0, 255); //점등
          strip.show();
          delay(150);
        }
        LightOn = true; //불 켜져 있음 표시
        Serial.println("불이 켜져있습니다.");
      }
      else
      {
        if (brightness < 150)
        {
          brightness += 70;
          strip.setBrightness(brightness);
          strip.show();
        }
      }
}

void TurnOffLight()
{
  if (brightness >= 80 && LightOn == true)
      {
        brightness -= 70;
        strip.setBrightness(brightness);
        strip.show();
      }
      else
      {
        for (i = 0; i < 24; i++)
        {
          strip.setPixelColor(i, 0, 0, 0, 0); // 소등
          strip.show();
          delay(50);
        }
        LightOn = false;
        Serial.println("불이 꺼졌습니다.");
        stage = 1; //기본 스테이지로 초기화.
      }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//제스쳐 함수
void interruptRoutine()
{
  isr_flag = 1;
}

void handleGesture()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      Serial.println("UP");
      TurnOnLight();
      break;

    case DIR_DOWN:
      TurnOffLight();
      break;
    case DIR_LEFT:
      Serial.println("LEFT");
      if (stage > 0) //스테이지 범위안에서만
      {
        stage--;
        ChangeColor(); //색 바꿔줍니다
      }
      break;
    case DIR_RIGHT:
      Serial.println("RIGHT");
      if (stage < 3)
      {
        stage++;
        ChangeColor();
      }
      break;
    case DIR_NEAR:
      Serial.println("NEAR");
      turnbacklight();
      break;
    case DIR_FAR:
      Serial.println("FAR");
      turnbacklight();
      break;
    default:
      Serial.println("NONE");
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//블루투스 함수

void getbtstring(){
  while(btSerial.available()) {
    char datachar = (char)btSerial.read();
    if(data.length() == 6) 
    {
      btserialFlush();
      Serial.println("6개라서 끊어준다");
      break;
    }
    if(datachar == 13)          // /n 과 라인체인지값 빼준다. 
    {
      continue;
    }
    if(datachar == 10)
    {
      continue;
    }
    data += datachar;
    delay(5);
  }
  Serial.println(data); //끝나고 읽은 값 출력해줌
  delay(300);
}

void bluetoothonoff(){
  if(!data.equals(""))  //myString 값이 있다면
  {
    if(data == "on")
    {
      TurnOnLight();
    }
    else if(data == "off")
    {
      TurnOffLight();
    }
    else                    //색 정보 보내줬을 경우
    {
      String red = data.substring(0, 2);
      String green = data.substring(2, 4);
      String blue = data.substring(4, 6);

      char r[3];
      red.toCharArray(r, 3);
      char g[3];
      green.toCharArray(g, 3);
      char b[3];
      blue.toCharArray(b, 3);

      Serial.println(r);
      Serial.println(g);
      Serial.println(b);

      int rhex = strtol(r, NULL, 16);
      int ghex = strtol(g, NULL, 16);
      int bhex = strtol(b, NULL, 16);

      colorWipe(strip.Color(rhex, ghex, bhex, 0), 0);
    }
    data="";  //myString 변수값 초기화
  }
}

