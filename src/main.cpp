//자동정렬 shift + alt + f

#include <Arduino.h>
#include "bluetooth.h"
#include "gesture.h"
#include "neopixel.h"
#include "dustsensor.h"

#include <LiquidCrystal_I2C.h>      //LiquidCrystal 라이브러리 추가
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd 객체 선언

//전역변수
int turn = 0;
int VolumeState = 0;

//블루투스 관련 선언
String data = "";
void getbtstring();
void bluetoothonoff();
void btserialFlush();

//제스쳐관련 선언
void handleGesture();

//네오픽셀관련 선언
void ChangeColor();
void TurnOnLight();
void TurnOffLight();

void setup()
{
  btSerial.begin(9600);
  Serial.begin(9600);

  lcd.init(); // LCD 초기화
  lcd.backlight();
  lcd.setCursor(0, 0); // 1번째, 1라인
  lcd.print("Turning On");
  lcd.setCursor(0, 1); // 1번째, 2라인
  lcd.print("Gesture Sensor..");

  randomSeed(analogRead(0)); //랜덤시드 for gestrue light changing system

  strip.setBrightness(brightness);
  strip.begin();
  strip.show();

  pinMode(APDS9960_INT, INPUT);
  pinMode(sharpLEDPin, OUTPUT);

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
  lcd.init();
}
int VoRaw;
float Vo;

void loop()
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

  // int Register = analogRead(A0);
  // if( Register>300 && Register<500){
  //   lcd.init();
  // }

  // lcd.setCursor(0, 0); // 1번째, 2라인
  // lcd.print("Gesture Running");
  // lcd.setCursor(0, 1); // 1번째, 2라인
  // lcd.print(turn);
  // turn++;

  // if (isr_flag == 1)
  // {
  //   detachInterrupt(0);
  //   handleGesture();
  //   isr_flag = 0;
  //   attachInterrupt(0, interruptRoutine, FALLING);
  // }
  // getbtstring();
  // bluetoothonoff();

  // if(VolumeState == 4){
  //    //whiteOverRainbow(102, 2);
  //    rainbowFade2White(3,3,0);
  // }

  // if(LightState == false){
  //   pulseWhite(5);
  //   //whiteOverRainbow(100, 2);
  // }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//네오픽셀 함수

void ChangeColor()
{
  if (LightState == true)
  {
    long randNumber1 = random(255);
    long randNumber2 = random(255);
    long randNumber3 = random(255);
    //long randNumber4 = random(255);

    colorWipe(strip.Color(randNumber1, randNumber2, randNumber3, 0), 30);
  }
}

void TurnOnLight()
{
  if (LightState == false) //꺼져있을때만 점등
  {
    strip.begin();
    for (int i = 0; i < 24; i++)
    {
      strip.setPixelColor(i, 0, 0, 0, 255); //점등
      strip.show();
      delay(150);
    }
    LightState = true; //불 켜져 있음 표시
    Serial.println("불이 켜져있습니다.");
  }
  else
  {
    if (brightness < 127.5)
    {
      brightness += 127.5;
      strip.setBrightness(brightness);
      strip.show();
    }
  }
}

void TurnOffLight()
{
  if (brightness >= 127.5 && LightState == true)
  {
    brightness -= 127.5;
    strip.setBrightness(brightness);
    strip.show();
  }
  else
  {
    for (int i = 0; i < 24; i++)
    {
      strip.setPixelColor(i, 0, 0, 0, 0); // 소등
      strip.show();
      delay(50);
    }
    LightState = false;
    Serial.println("불이 꺼졌습니다.");
    ColorState = 1; //기본 스테이지로 초기화.
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//제스쳐 함수

void handleGesture()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      if (VolumeState == 4)
      {
      }
      else
      {
        Serial.println("UP");
        TurnOnLight();
        VolumeState++;
      }
      break;

    case DIR_DOWN:
      TurnOffLight();
      VolumeState--;
      break;
    case DIR_LEFT:
      Serial.println("LEFT");
      ColorState--;
      ChangeColor();
      break;
    case DIR_RIGHT:
      Serial.println("RIGHT");
      ColorState++;
      ChangeColor();
      break;
    // case DIR_NEAR:
    //   Serial.println("NEAR");
    //   break;
    // case DIR_FAR:
    //   Serial.println("FAR");
    //   break;
    default:
      Serial.println("NONE");
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//블루투스 함수

void getbtstring()
{
  while (btSerial.available())
  {
    char datachar = (char)btSerial.read();
    if (data.length() == 6)
    {
      // btserialFlush();
      Serial.println("6개라서 끊어준다");
      break;
    }
    if (datachar == 13) // /n 과 라인체인지값 빼준다.
    {
      continue;
    }
    if (datachar == 10)
    {
      continue;
    }
    data += datachar;
    delay(5);
  }
  Serial.println(data); //끝나고 읽은 값 출력해줌
  delay(300);
}

void bluetoothonoff()
{
  if (!data.equals("")) //myString 값이 있다면
  {
    if (data == "on")
    {
      TurnOnLight();
    }
    else if (data == "off")
    {
      TurnOffLight();
    }
    else //색 정보 보내줬을 경우
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
    data = ""; //myString 변수값 초기화
  }
}

// void btserialFlush(){
//   while(btSerial.available() > 0) {
//     char t = btSerial.read();
//   }
// }
