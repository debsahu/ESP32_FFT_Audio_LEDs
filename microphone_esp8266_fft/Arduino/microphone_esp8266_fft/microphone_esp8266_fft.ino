#include <arduinoFFT.h>      //https://github.com/kosme/arduinoFFT
//                           //https://github.com/G6EJD/ESP32-8266-Audio-Spectrum-Display

#define NUM_BANDS  8
#define READ_DELAY 50
#define USE_RANDOM_DATA false

#include <WS2812FX.h>        //https://github.com/kitesurfer1404/WS2812FX
#include "custom/VUMeter.h"  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

arduinoFFT FFT = arduinoFFT();

#define SAMPLES 512              //Must be a power of 2
#define SAMPLING_FREQUENCY 40000 //Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT.

// Use ADC1 so that WiFi stuff doesnt interfere with ADC measurements
#define ADC_PIN 36 // 36 = PIN VP on Lolin D32

int amplitude = 200;
unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0, 0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;

// LED Settings
#define LED_COUNT 120
#define LED_PIN   12

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void displayBand(int band, int dsize)
{
  int dmax = amplitude;
  if (dsize > dmax)
    dsize = dmax;
  if (dsize > peak[band])
  {
    peak[band] = dsize;
  }
}

void setup()
{
  Serial.begin(256000);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  pinMode(ADC_PIN, INPUT);

  ws2812fx.init();
  ws2812fx.setBrightness(32);

  // setup the custom effect
  uint32_t colors[] = {GREEN, YELLOW, RED};
  uint8_t vuMeterMode = ws2812fx.setCustomMode(F("VU Meter"), vuMeter);
  ws2812fx.setSegment(0, 0, LED_COUNT-1, vuMeterMode, colors, READ_DELAY, NO_OPTIONS);

  ws2812fx.start();
}

void loop()
{
  for (int i = 0; i < SAMPLES; i++)
  {
    newTime = micros() - oldTime;
    oldTime = newTime;
    vReal[i] = analogRead(ADC_PIN); // A conversion takes about 1mS on an ESP8266
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us))
    {
      delay(0);
    }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  for (int i = 2; i < (SAMPLES / 2); i++)
  { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
    if (vReal[i] > 2000)
    { // Add a crude noise filter, 10 x amplitude or more
      if (i <= 2)
        displayBand(0, (int)vReal[i] / amplitude); // 125Hz
      if (i > 3 && i <= 5)
        displayBand(1, (int)vReal[i] / amplitude); // 250Hz
      if (i > 5 && i <= 7)
        displayBand(2, (int)vReal[i] / amplitude); // 500Hz
      if (i > 7 && i <= 15)
        displayBand(3, (int)vReal[i] / amplitude); // 1000Hz
      if (i > 15 && i <= 30)
        displayBand(4, (int)vReal[i] / amplitude); // 2000Hz
      if (i > 30 && i <= 53)
        displayBand(5, (int)vReal[i] / amplitude); // 4000Hz
      if (i > 53 && i <= 200)
        displayBand(6, (int)vReal[i] / amplitude); // 8000Hz
      if (i > 200)
        displayBand(7, (int)vReal[i] / amplitude); // 16000Hz
    }
  }
  if (millis() % 4 == 0)
  {
    for (byte band = 0; band < NUM_BANDS; band++)
    {
      if (peak[band] > 0)
        peak[band] /= 2;
    }
  } // Decay the peak
  for (byte band = 0; band < NUM_BANDS; band++)
  {
    uint16_t value = peak[band];
    vuMeterBands[band] = value < 1 ? 0 : map(value, 1, amplitude, 0, 255);
    Serial.print(value);
    Serial.print("\t");
  }
  Serial.println();

  ws2812fx.service();
}
