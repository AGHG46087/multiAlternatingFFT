/*
 * Multi alternating FFT
 * changes from a 6x10 graph to a 10x6 graph - bottom up and top down
 * FFT for LED sequrntial Strip, that could be used in horizonal or even vertical plane
 * by Hans-Jurgen Greiner
 * January 2019
 * 
 * based on FFT library and code from the Arduino forums
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
int HEIGHT = NUM_PIXELS/10;
int SEG = NUM_PIXELS/6;

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
  int ledIndex = 0;
  int lclVal = 0;

  VU_Controller.run(); // hit the Thread Controller run method.

    // Rotate the frequency,  so it is a different position every so many cycles
  if ( ++freqCounter > 1024 ) { rotateFreqs--;  }
  if (rotateFreqs <= 0 ) { rotateFreqs = NUM_FREQS; freqCounter = 0; }


  for (i=0; i < 128; i++){ 
    lclVal = 5000 * analogRead(AUDIOPIN); // analog sig not detected from laptop audio out. scale to 5000  
    lclVal = abs((int)lclVal % 128);
    data[i] = lclVal;
    im[i] = 0;
  };

    /* 
     *  Frequency Sprectum this bad boy
     * 1. data array
     * 2. imaginary number array
     * 3. FFT requires samples in power of two, we have a 128 so in our case (2^7 == 128)
     * 4. Invert, We will always say false here
     */ 
  fix_fft(data,im,7,0);  

  for (i=0; i< 64;i++){                                      
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  // this gets the absolute value of the values in the array, so we're only dealing with positive numbers
  };     
  
  
    // average bars together
  for (i=0; i<14; i++) {
    data_avgs[i] = data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];   // average together 
    data_avgs[i] = map(data_avgs[i], 0, 30, 0, HEIGHT);                         // remap values
  }
  
  
  
    // set LoLShield
  offsetGreenHue += HUE_STEP_INCREMENT;
  offsetBlueHue += HUE_STEP_INCREMENT;
  if (offsetGreenHue >= HUE_MAX) offsetGreenHue = 0;
  if (offsetBlueHue >= HUE_MAX) offsetBlueHue = 0;

  for (int x=0; x < SEG; x++) {
    for (int y=0; y < HEIGHT; y++) {
      ledIndex = (x * HEIGHT) + y; 
      if(reverse) { 
          // Bump it a segment higher and take off the height
        ledIndex = ((x+1) * HEIGHT) - y; 
      }
//      if (y < data_avgs[13-x]) { // 13-x reverses the bars so low to high frequences are represented from left to right.
      if (y < data_avgs[rotateFreqs-x]) { // 13-x reverses the bars so low to high frequences are represented from left to right.
        strip.setPixelColor(ledIndex, Wheel(map(data_avgs[y],0,HEIGHT,(int)offsetGreenHue, (int)offsetBlueHue)));
      } else {
        strip.setPixelColor(ledIndex, strip.Color(0,0,0));
      }
    } 
  }

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
    // Ok let reset the timer to a different random time,  surprise us.
  int randNumber = random(3,10);
  segmentThread.setInterval(randNumber * MILLISECONDS);
//  Serial.print("Callback: Segment Change will be in : "); Serial.println(randNumber);
  segmentThread.enabled = true;
}
