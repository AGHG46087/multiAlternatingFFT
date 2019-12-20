/*
 * Multi alternating FFT
 * changes from a 6x10 graph to a 10x6 graph - bottom up and top down
 * FFT for LED sequrntial Strip, that could be used in horizonal or even vertical plane
 * by Hans-Jurgen Greiner
 * 2019
 * 
 * base code based on FFT library and code from the Arduino forums
 * 
 */
#include <Adafruit_NeoPixel.h>
#include <fix_fft.h>
#include <Thread.h>
#include <ThreadController.h>

#define AUDIOPIN A5
#define NUM_PIXELS 60
#define LED_PIN  6
#define NUM_FREQS 13
#define HUE_STEP_INCREMENT 20
#define HUE_MAX 255
const int MILLISECONDS = 1000;
const int REVERSE_THREAD_INTERVAL = 5 * MILLISECONDS;  // Seconds
