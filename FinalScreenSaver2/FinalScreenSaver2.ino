

#include <Adafruit_NeoPixel.h>
#include <stdlib.h> 
#include <Time.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            31

#define BUTTON_PIN   2

#define BRIGHT_PIN            A14

int NUMPIXELS = 60;

int delayval = 0; 

int runStrip = 0;

int brightness = 100;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.print("Hello!");
  Serial.print("\n");
  pixels.begin();
  pixels.show();
  pixels.setBrightness(brightness);

}

void loop() {

  //pixels.show();
  bool newState = digitalRead(BUTTON_PIN);
  if (newState == HIGH) 
  {
    // Short delay to debounce button.
    delay(100);
    newState = digitalRead(BUTTON_PIN);
    if (newState == HIGH) 
    {
      runStrip = 1;
      Serial.print(runStrip);
      Serial.print("\n");
    }

  }
  if(runStrip==1)
    {
      colorFade(25, 100, 0, 0, 0);
      runStrip = 0;
      Serial.print(runStrip);
      Serial.print("\n");
      pixels.show();
    }
  }
  void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
}

int roundUp(int toRound)
{
     if (toRound % 10 == 0) return toRound;
     return (10 - toRound % 10) + toRound;
}

int roundDown(int toRound)
{
    return toRound - toRound % 10;
}

void bright() {
  //Serial.print("Pin read: \t"); Serial.print(nVal); Serial.print("\n");
  int bhs = analogRead(BRIGHT_PIN);
  bhs = roundUp(bhs);
  bhs = roundDown(bhs);
  bhs = constrain(bhs,20,680);
  bhs = map(bhs,20,680,0,255);
  brightness = bhs;
  pixels.setBrightness(brightness);
  //Serial.print("Pin read: \t"); Serial.print(bhs); Serial.print("\n");
}

void setAll(int red, int green, int blue) {
  char buff[256];
  sprintf(buff, "Red %d, Green %d, Blue %d\n", red, green, blue);
  Serial.print(buff);
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
  void colorFade(int waitTime, int maxColor, int r, int g, int b) {
    while(1) {
      int color = random(0,3);
      int colorAmount = random(0,maxColor);
      bright();
      
      if(color==0) { //Red
        if(r>colorAmount) {
          for(r=r;r>colorAmount;r--) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
        else {
          for(r=r;r<colorAmount;r++) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
      }
      if(color==1) { //Green
        if(g>colorAmount) {
          for(g=g;g>colorAmount;g--) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
        else {
          for(g=g;g<colorAmount;g++) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
      }
      if(color==2) { //Blue
        if(b>colorAmount) {
          for(b=b;b>colorAmount;b--) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
        else {
          for(b=b;b<colorAmount;b++) {
            setAll(r,g,b);
            delay(waitTime);
          }
        }
      }
      
      showStrip();
    }
    
    
                    }
