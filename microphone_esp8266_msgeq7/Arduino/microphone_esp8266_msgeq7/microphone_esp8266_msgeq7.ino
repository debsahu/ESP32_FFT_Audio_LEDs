#include <MD_MSGEQ7.h>      //https://github.com/debsahu/MD_MSGEQ7

#define NUM_BANDS MAX_BAND
#define USE_RANDOM_DATA false

#include <WS2812FX.h>       //https://github.com/kitesurfer1404/WS2812FX
#include "custom/VUMeter.h" //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// hardware pin definitions - change to suit circuit
#define DATA_PIN    34
#define RESET_PIN   27
#define STROBE_PIN  26

// frequency reading the IC data
#define READ_DELAY  50

// LED Settings
#define LED_COUNT 120
#define LED_PIN   12

MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(256000);
  Serial.println("[MD_MSG_SEQ7_Serial]");
  MSGEQ7.begin();
  analogReadResolution(10);

  ws2812fx.init();
  ws2812fx.setBrightness(255);

  // setup the custom effect
  uint32_t colors[] = {GREEN, YELLOW, RED};
  uint8_t vuMeterMode = ws2812fx.setCustomMode(F("VU Meter"), vuMeter);
  ws2812fx.setSegment(0, 0, LED_COUNT-1, vuMeterMode, colors, READ_DELAY, NO_OPTIONS);

  ws2812fx.start();
}

void loop() 
{
  static long then = 0;
  long now = millis();

  if(now > then + READ_DELAY) {

    MSGEQ7.read();

    // Serial output
    for (uint8_t i=0; i<MAX_BAND; i++)
    {
      uint16_t value = MSGEQ7.get(i);
      vuMeterBands[i] = value < 40 ? 0 : map(value, 40, 1023, 0, 255);
      Serial.print(value);
      Serial.print('\t');
    }
    Serial.println();
    then = now;
  }

  ws2812fx.service();
}