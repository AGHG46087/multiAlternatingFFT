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

long lclTime = millis();

// multiAlternatingFFT

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);


int freqCounter = 0;
int rotateFreqs = NUM_FREQS;
char im[128], data[128]; // FFT requires an IM-aginary part and DATA part. 
char data_avgs[14];
int i=0,val;
bool reverse = false;
bool segmentOne = true;  // Segment one is true, segment two is false.  

int offsetGreenHue = 30;
int offsetBlueHue = 150;
