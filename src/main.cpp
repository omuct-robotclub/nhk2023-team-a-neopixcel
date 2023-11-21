#include <Arduino.h>

#include <Adafruit_NeoPixel.h> // NeoPixel制御用ライブラリの準備

// NeoPixel WS2812Bの指定
#define NUM_LEDS 68 // NeoPixelの数
static int16_t neopixcelPins[] = {3, 4, 5, 11};
static uint8_t neopixcelLen = sizeof(neopixcelPins) / sizeof(neopixcelPins[0]);
static uint8_t readStatePins[] = {20, 26, 28};
static uint8_t readStateLen = sizeof(readStatePins) / sizeof(readStatePins[0]);
static unsigned long timeStart = millis();
static unsigned long timeEnd = millis();
// NeoPixcelライブラリの設定、このまま書いておく-----------------------
// NeoPixel用の stripオブジェクトを宣言（LEO_GRB + NEO_KHZ800 は NeoPixel WS2812 使用の場合の設定）

Adafruit_NeoPixel strip[] = {Adafruit_NeoPixel(NUM_LEDS, neopixcelPins[0], NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(NUM_LEDS, neopixcelPins[1], NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(NUM_LEDS, neopixcelPins[2], NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(NUM_LEDS, neopixcelPins[3], NEO_GRB + NEO_KHZ800)};

struct ColorState
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
ColorState colorState = {0, 0, 0};

// 初期設定、このまま書いておく --------------------------------------
void setup()
{
  // NeoPixel(WS2812)初期設定
  Serial.begin(115200);
  for (uint8_t i = 0; i < neopixcelLen; i++)
  {
    strip[i].begin();            // NeoPixel初期化
    strip[i].show();             // NeoPixcelを全て消灯
    strip[i].setBrightness(255); // 明るさ倍率設定 (RGB設定値 * 明るさ倍率、最大255)
  }
  for (uint8_t i = 0; i < readStateLen; i++)
  {
    pinMode(readStatePins[i], INPUT);
  }
}
// 以下に点灯させる処理を記入-----------------------------------------
// LEDテープライトを同じ色で全点灯
void loop()
{
  Serial.print(digitalRead(readStatePins[0]));
  Serial.print(digitalRead(readStatePins[1]));
  Serial.print(digitalRead(readStatePins[2]));
  bool readPin0 = digitalRead(readStatePins[0]);
  bool readPin1 = digitalRead(readStatePins[1]);

  // Serial.print(readPin0);
  // Serial.println(readPin1);
  timeEnd = millis();
  if (digitalRead(readStatePins[2]))
  {
    timeStart = millis();
    colorState = {0, 0, 0};
    Serial.println("off");
  }
  else if(timeEnd-timeStart<1000){
    colorState = {0, 0, 0};
    Serial.println("fakeoff");
  }
  else if (readPin1)
  {
    colorState = {255,30,30};
    Serial.println("red");
  }
  else if (readPin0)
  {
    colorState = {30, 30, 255};
    Serial.println("blue");
  }                                                                                         

  // for文で全てのNeoPixelのRGB（赤緑青）の明るさを0〜255（30程でも十分明るい）で指定
  for (uint8_t len = 0; len < neopixcelLen; len++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      strip[len].setPixelColor(i, strip[len].Color(colorState.r, colorState.g, colorState.b)); // LED発光色指定（num,strip.Color（R,G,B））※RGB:0〜255
    }
    strip[len].show(); // 点灯実行（NeoPixel制御パルス出力）
  }
  delay(100);
}