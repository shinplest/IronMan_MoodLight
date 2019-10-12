#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>

// Pins
#define APDS9960_INT 2 // Needs to be an interrupt pin

// Constants
#define LIGHT_INT_HIGH 1000 // High light level for interrupt
#define LIGHT_INT_LOW 10    // Low light level for interrupt

// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;
uint16_t threshold = 0;

//method
void interruptRoutine()
{
  isr_flag = 1;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6          // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24    // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)
#define BRIGHTNESS 255 // 네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)

int i;//네오픽셀 변수

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>     //LiquidCrystal 라이브러리 추가 
LiquidCrystal_I2C lcd(0x27, 16, 2);  //lcd 객체 선언



void setup()
{

  strip.setBrightness(BRIGHTNESS);
  strip.begin(); // 네오픽셀 제어 시작
  strip.show();  // 네오픽셀 점등

  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - Light Interrupts"));
  Serial.println(F("-------------------------------------"));

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

  // Set high and low interrupt thresholds
  if (!apds.setLightIntLowThreshold(LIGHT_INT_LOW))
  {
    Serial.println(F("Error writing low threshold"));
  }
  if (!apds.setLightIntHighThreshold(LIGHT_INT_HIGH))
  {
    Serial.println(F("Error writing high threshold"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if (apds.enableLightSensor(false))
  {
    Serial.println(F("Light sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  // Read high and low interrupt thresholds
  if (!apds.getLightIntLowThreshold(threshold))
  {
    Serial.println(F("Error reading low threshold"));
  }
  else
  {
    Serial.print(F("Low Threshold: "));
    Serial.println(threshold);
  }
  if (!apds.getLightIntHighThreshold(threshold))
  {
    Serial.println(F("Error reading high threshold"));
  }
  else
  {
    Serial.print(F("High Threshold: "));
    Serial.println(threshold);
  }

  // Enable interrupts
  if (!apds.setAmbientLightIntEnable(1))
  {
    Serial.println(F("Error enabling interrupts"));
  }

  // Wait for initialization and calibration to finish
  delay(500);


  lcd.init(); // LCD 초기화
  // Print a message to the LCD.
  lcd.backlight();     // 백라이트 켜기
  lcd.setCursor(0, 0); // 1번째, 1라인
  lcd.print("2019-10-17 01:18");
  lcd.setCursor(0, 1); // 1번째, 2라인
  lcd.print("MISE MUNG:200ppm");
}

void loop()
{
  // If interrupt occurs, print out the light levels
  if (isr_flag == 1)
  {

    // Read the light levels (ambient, red, green, blue) and print
    if (!apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light))
    {
      Serial.println("Error reading light values");
    }
    else
    {
      if (ambient_light < 10)
      {
        strip.begin();
        // strip.setPixelColor(0, 255, 0, 0, 0); // 첫 번째 숫자: 0번째(1번) 픽셀 소자, 0=1번째 픽셀, 1=2번째 픽셀 ...
        //strip.setPixelColor(1, 0, 255, 0, 0); // 둘째, 셋제, 넷째는 R(RED), G(GREEN), B(BLUE), W(White)값, 0~255사이 표현
        //strip.setPixelColor(2, 0, 0, 255, 0); // 3번째 픽셀에 B(Blue)소자를 255로 켜기
        for(i = 0; i < 24; i++){
          strip.setPixelColor(i, 0, 0, 0, 255); // 4번째 픽셀에 W(White)소자를 255로 켜기
          strip.show();
          delay(150);
        }

      }
      else
      {
        for(i = 0; i < 24; i++){
          strip.setPixelColor(i, 0, 0, 0, 0); // 4번째 픽셀에 W(White)소자를 255로 켜기
          strip.show();
          delay(50);
        }
      }

      Serial.print("Interrupt! Ambient: ");
      Serial.print(ambient_light);
      Serial.print(" R: ");
      Serial.print(red_light);
      Serial.print(" G: ");
      Serial.print(green_light);
      Serial.print(" B: ");
      Serial.println(blue_light);
    }

    // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
    isr_flag = 0;
    if (!apds.clearAmbientLightInt())
    {
      Serial.println("Error clearing interrupt");
    }
  }
}