/* 본 코드는 아토플래닛 주식회사와 씽코딩이 제공하는
 * 네오픽셀 교육용 테스트 코드입니다. 학습 및 연구에 도움이 되길 바랍니다.^^
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 5 // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24 // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)
#define BRIGHTNESS 255 // 네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin(); // 네오픽셀 제어 시작
  strip.show(); // 네오픽셀 점등
}

void loop() {
  strip.begin();
  strip.setPixelColor(0,255,0,0,0); // 첫 번째 숫자: 0번째(1번) 픽셀 소자, 0=1번째 픽셀, 1=2번째 픽셀 ...
  strip.setPixelColor(1, 0,255,0,0); // 둘째, 셋제, 넷째는 R(RED), G(GREEN), B(BLUE), W(White)값, 0~255사이 표현
  strip.setPixelColor(2, 0,0,255,0); // 3번째 픽셀에 B(Blue)소자를 255로 켜기
  strip.setPixelColor(3, 0,0,0,255); // 4번째 픽셀에 W(White)소자를 255로 켜기
  strip.show();
  delay(8000);

  colorWipe(strip.Color(255,0,0,0),0); //  colorWipe함수를 이용하면 모든 네오픽셀을 동일한 색상으로 변경
  delay(2000);
  colorWipe(strip.Color(0,255,0,0),0); // 소괄호 내 4개 숫자가 RGBW
  delay(2000);
  colorWipe(strip.Color(0,0,255,0),100); // 뒤의 0은 딜레이 타임으로, 숫자를 넣으면 픽셀이 하나 하나씩 천천히 색상이 바뀜
  delay(2000);
  colorWipe(strip.Color(0,0,0,255),1500);
  delay(2000);
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
