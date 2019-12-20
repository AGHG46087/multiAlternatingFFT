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

// Just allocate here, since we need a global declaration
// We are going to set it as a standard object rather than a pointer to a object. Difference is dot notation.
//Thread* reverseThread = new Thread(); 
Thread reverseThread = Thread(); 
Thread segmentThread = Thread();

// We are doing a complete controller in the case I decide to add new Threads in the future it will be ready.
// With just one it is overkill, but easier setup now rather than later.
ThreadController VU_Controller = ThreadController();

void setup() {
  strip.begin();
  strip.show(); // all pixels to 'off'
  Serial.begin(57600); // We all know the drill on the log stuff.

    // if analog input pin 0 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    // Special treat to randomize 6 or 10 pattern seconds
  randomSeed(analogRead(0));
  int randNumber = random(3,10);
//  Serial.print("init: Segment Change will be in : "); Serial.println(randNumber);

    // Here is my Thread setup
  reverseThread.onRun(reverseToggle);
  reverseThread.setInterval(REVERSE_THREAD_INTERVAL);

  segmentThread.onRun(segmentChange);
  segmentThread.setInterval(randNumber * MILLISECONDS);

  VU_Controller.add(&reverseThread);
  VU_Controller.add(&segmentThread);

  
  delay(3000);   // SPOOL UP TIME 

}

void loop() {

  strip.show();
}

// Thanks Fred Z. this works pretty good.
// Input a value 0 to HUE_MAX, returns color value.
// The rotates over color RGB 
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void reverseToggle() {
  reverse = !reverse;
//  Serial.print("Callback: Reverse value is now: "); Serial.println(reverse);
}
void segmentChange() {
  segmentThread.enabled = false;
  long now = millis();
//  Serial.print("Callback: Time Passed : "); Serial.println(now - lclTime);
  lclTime = now;
  
    // Every so many cycles, change the graph from a 6x10 to a 10x6 display
  if( segmentOne ) {
    HEIGHT = NUM_PIXELS/6;
    SEG = NUM_PIXELS/10;
  }
  else {
      // Reset the graph to a 6x10
    HEIGHT = NUM_PIXELS/10;
    SEG = NUM_PIXELS/6;
  }
}
