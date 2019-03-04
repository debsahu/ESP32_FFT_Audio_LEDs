# ESP32 Fast Fourier Transform of Audio & WS2812B mapping

[![License: MIT](https://img.shields.io/github/license/debsahu/ESP32_FFT_Audio_LEDs.svg)](https://opensource.org/licenses/MIT) [![LastCommit](https://img.shields.io/github/last-commit/debsahu/ESP32_FFT_Audio_LEDs.svg?style=social)](https://github.com/debsahu/ESP32_FFT_Audio_LEDs/commits/master)

Here we use ESP32 microcontroller to analyze real-time audio signals
- Real-time FFT to get frequencies (99% of code from [ESP32-8266-Audio-Spectrum-Display Project](https://github.com/G6EJD/ESP32-8266-Audio-Spectrum-Display) by [**@G6EJD**](https://github.com/G6EJD))
- Or use MSGEQ7 chip to get the same data
- Display frequency bands on LEDs as bars

[![ESP32_FFT_AUDIO_LEDs](https://img.youtube.com/vi/XXXXXXXXXXX/0.jpg)](https://www.youtube.com/watch?v=XXXXXXXXXXX)

## Libraries Needed

[Software FFT platformio.ini](https://github.com/debsahu/ESP32_FFT_Audio_LEDs/blob/master/microphone_esp8266_fft/platformio.ini) and [Hardware FFT platformio.ini](https://github.com/debsahu/ESP32_FFT_Audio_LEDs/blob/master/microphone_esp8266_msgeq7/platformio.ini) is included, use [PlatformIO](https://platformio.org/platformio-ide) and it will take care of installing the following libraries.

| Library                   | Link                                                       |
|---------------------------|------------------------------------------------------------|
|arduinoFFT                 |https://github.com/kosme/arduinoFFT                         |
|WS2812FX                   |https://github.com/kitesurfer1404/WS2812FX                  |
|MD_MSGEQ7                  |https://github.com/debsahu/MD_MSGEQ7                        |

## Known Limitations

- Sampling frequency must be high
- Sampling time should not interfere with other WiFi functions
- FFT calculations are CPU intensive
- Frequency binning needs to be done manually

## Wiring

![ww_wiring](https://github.com/debsahu/ESP32_FFT_Audio_LEDs/blob/master/docs/software_fft.png)

![hw_wiring](https://github.com/debsahu/ESP32_FFT_Audio_LEDs/blob/master/docs/hardware_fft.png)
