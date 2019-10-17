#include <SoftwareSerial.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//네오픽셀관련 선언

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5          // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24    // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)

int i;                          //네오픽셀 변수
boolean LightOn = false;        //전원 여부 판단
int r = 0, g = 0, b = 0, w = 0; //색 밝기 저장
int stage = 1;                  //0 = red, 1 = white, 2 = green , 3 = blue
int brightness = 10; // 네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);




int Tx=6; //전송
int Rx=7; //수신 
SoftwareSerial btSerial(Tx, Rx);
String data = "";
 
void setup() 
{
  Serial.begin(9600);
  btSerial.begin(9600);

  strip.setBrightness(brightness);
  strip.begin(); // 네오픽셀 제어 시작
  strip.show();  // 네오픽셀 점등
  
}

void getbtstring(){
  while(btSerial.available()) {
    char datachar = (char)btSerial.read();
    if(datachar == 13)
    {
      Serial.println("13번실행");
      continue;
    }
    if(datachar == 10)
    {
      Serial.println("10번실행");
      continue;
    }
    data += datachar;
    Serial.println("실행");
    delay(5);
  }
}
 
void loop()
{
  getbtstring();

  if(!data.equals(""))  //myString 값이 있다면
  {
    if(data == "aa00aa")
    {
      
      strip.setPixelColor(10, 0, 0, 0, 255); //점등
      strip.show();
    }
 
    data="";  //myString 변수값 초기화
  }
}
