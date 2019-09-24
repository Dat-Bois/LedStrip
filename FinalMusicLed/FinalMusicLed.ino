

#include <Adafruit_NeoPixel.h>
#include <stdlib.h> 
#include <Time.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define MUSIC_PIN A16

#define BRIGHT_PIN            A14

#define PIN            31

#define BUTTON_PIN   2

#define NUMPIXELS  60

#define middle 30

int brightness = 100;

int diff = 0;

int preVal = 0;

int val = 0;

int delayval = 0; 

int runStrip = 0;

int r = 0;

int g = 0;

int b = 0;

int n=1;

int nPrev=0;

int l=1;

int lPrev=0;

const int ledPin = 13;

uint32_t newColor;

int beats[] = {1,2,3,4,5,6,7,8,9,10};

int readPin();

void subEffectPicker(String code, int intensity = readPin());

void logger(char *p)
{
  Serial.print(p);
  Serial.print("\n");
  }


uint32_t a[NUMPIXELS];
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t black = pixels.Color(0,0,0);



/*{
    0x9966CC,
    0xFFD700,
    0x9ACD32};
*/

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MUSIC_PIN, INPUT_PULLUP);
  pinMode(BRIGHT_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  randomSeed(analogRead(3));
  Serial.print("Hello!");
  Serial.print("\n");
  pixels.begin();
  //setPixel(0,150,0,0);
  pixels.show();
  pixels.setBrightness(brightness);
  
}

void loop() {
   
  //val = readPin();
  digitalWrite(ledPin, HIGH);
  pixels.setBrightness(brightness);
  /*
  HOW TO CALL EACH EFFECT: NOTE****** runTime is in seconds
  val = analogRead(MUSIC_PIN);
  Strobe(random(3,5), 25); // runTime - flashDelay
  theaterChase(musicColor(val), 25, random(5,10)); // color - wait - runTime
  colorSpread(random(10,20));// - runTime
  StrobeAll(0xff, 0xff, 0xff, 10, 50); // r-g-b - strobeCount - FlashCount
  StrobeMiddle(int runTime, int FlashDelay, int middle2)
  brightMusic(runTime); //how long to run
  brightPulse(beat, count);//analog value at which to trigger pulse - how many times
  colorFade(25, 100, 0, 0, 0); // - waitTime - maxColor - r-g-b
  */
  bool newState = digitalRead(BUTTON_PIN);
  if(newState == HIGH) 
  {
    // Short delay to debounce button.
    delay(5);
    
    newState = digitalRead(BUTTON_PIN);
    if (newState == HIGH) 
    {
      //Serial.print("Hello!");
    }
  
  }
  
  arrMake(beats, 5);
  val = largest(beats, 5);
  //Serial.print("Val:   "); Serial.print(val); Serial.print("\n");
  effectPicker(val);
  //Serial.print(val);
  //Serial.print("\n");
  //delay(25);
}
int readPin() {
  int nVal =  analogRead(MUSIC_PIN);
  Serial.print("Pin read: \t"); Serial.print(nVal); Serial.print("\n");
  int bhs = analogRead(BRIGHT_PIN);
  bhs = roundUp(bhs);
  bhs = roundDown(bhs);
  bhs = constrain(bhs,20,680);
  bhs = map(bhs,20,680,0,255);
  brightness = bhs;
  pixels.setBrightness(brightness);
  //Serial.print("Pin read: \t"); Serial.print(bhs); Serial.print("\n");
  //Serial.print("Pin read: \t"); Serial.print(nValt); Serial.print("\n");
  return nVal;
}
uint32_t musicColor(int sensorVal) {
  int sensVal = constrain(sensorVal,0,300); //300   
  sensorVal = map(sensVal,0,300,0,45); //300
  static int rid, gid, bid = 0;
  if(sensorVal<=15) {
    rid=sensorVal*3;
    gid=255-sensorVal*17;
    bid=0;
    //r=80;
    //g=180;
    //b=100;
  }
  if(sensorVal>15 && sensorVal<30) { //170
    sensorVal = sensorVal-15;
    bid=sensorVal*17;
    rid=255-sensorVal*17;
    gid=0;
    //r=100;
    //g=80;
    //b=180;
  }
  if(sensorVal>30 && sensorVal<45) {    //170
    sensorVal = sensorVal-30;
    bid=sensorVal*17;
    gid=255-sensorVal*17;
    rid=0;
    //r=180;
    //g=100;
    //b=80;
  }
  //Serial.print(sensorVal);
  //Serial.print("\n");
  uint32_t cLr = pixels.Color(rid,gid,bid);
  return cLr;
}

void setPixel(int Pixel, byte q, byte w, byte y) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(q, w, y));
 #endif
}
void setPixel(int Pixel, uint32_t color) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.setPixelColor(Pixel, color);
 #endif
}

void setAll(int h, int f, int g) {
  /*
  char buff[256];
  sprintf(buff, "Red %d, Green %d, Blue %d\n", h, f, g);
  Serial.print(buff);
  */
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, h, f, g); 
  }
  showStrip();
}

void setAll(uint32_t color) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, color); 
  }
  showStrip();
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}
void theaterChase(uint32_t c, uint8_t wait, int runTime) {
  int cycles = runTime/0.4;
  for (int j=0; j<cycles; j++) {  
    for (int q=0; q < 5; q++) {
      for (uint16_t i=0; i < NUMPIXELS; i=i+5) {
        //int iiughf = readPin();
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
        pixels.setPixelColor(i+q+1, c);
        pixels.show();

      }
      //pixels.show();

      delay(wait);

      for (uint16_t i=0; i < NUMPIXELS; i=i+5) {
        //int iuokgrf = readPin();
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
        pixels.setPixelColor(i+q+1, 0);
      }
    }
  }
}
void Strobe(int runTime, int FlashDelay){
  int valt = readPin();
  uint32_t coLOr = musicColor(valt);
  int maxSize = (4/6)*NUMPIXELS;
  int minSize = (1/6)*NUMPIXELS;
  int size = random(10,40);
  int point = random(0,NUMPIXELS);
  int point1;
  point1 = point + size;
  int point2;
  point2 = point - size;
  int StrobeCount = runTime/0.2;
  //Serial.print(size);
  //Serial.print("\n");
  for(int j = 0; j < StrobeCount; j++) {
    if(point1<=NUMPIXELS) {
      //Serial.print("if");
      //Serial.print("\n");
      for(int i = point; i <= point1; i++ ) {
        //Serial.print(i);
        //Serial.print("\n");
        setPixel(i, coLOr); 
  }
}
    else {
      //Serial.print("else");
      //Serial.print("\n");
      for(int i = point; i >= point2; i-- ) {
        //Serial.print(i);
        //Serial.print("\n");
        setPixel(i, coLOr); 
  }
    }
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
}
void StrobeAll(byte rig, byte gig, byte big, int StrobeCount, int FlashDelay){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(rig,gig,big);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
}
void colorFade(int waitTime, int maxColor, int r, int g, int b) {
    while(1) {
      int color = random(0,3);
      int colorAmount = random(0,maxColor);
      
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
void colorSpread(int runTime) {
  for(int i=0;i<=runTime*1000;i=i+20){
    val = readPin();
    if (val >= 50)
      {
        a[middle] = musicColor(val);
      }
    for (int x = NUMPIXELS; x > middle; x--) { //set middle to 0 for space in between
        a[x] = a[x - 1];
  
     }
     for (int z = 0; z < middle; z++) { //set middle to 0 for space in between
        a[z] = a[z + 1];
  
     }
     
     for (int y = 1;y<NUMPIXELS;y++) {
      setPixel(y,a[y]);
     // pixels.show();
     }
    
    pixels.show();   
    delay(20);
    //a[middle] = black; // Comment line out if you don't want color to dissapear
}
}
void StrobeMiddle(int runTime, int FlashDelay, int middle2){
  int maxSize = (4/6)*NUMPIXELS;
  int minSize = (1/6)*NUMPIXELS;
  int StrobeCount = (runTime + 2)/0.2;
  for(int j = 0; j < StrobeCount; j++) {
  int valt = readPin();
  uint32_t coLOr = musicColor(valt);
  int valted = constrain(valt, 0, 300);
  int size = map(valted, 0, 300, 0, NUMPIXELS);
  int point = middle2;
  int point1;
  point1 = point + size/2; //+7
  int point2;
  point2 = point - size/2; //-7
  //Serial.print(size);
  //Serial.print("\n");
  for(int i = point; i <= point1; i++ ) {
    //Serial.print(i);
    //Serial.print("\n");
    setPixel(i, coLOr); 
  }
  for(int i = point; i >= point2; i-- ) {
    //Serial.print(i);
    //Serial.print("\n");
    setPixel(i, coLOr); 
  }
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
}
void effectPicker(int valter) {
  int vzy;
  nPrev = n;
  //Serial.print(valter);
  //Serial.print("\n");

  if(valter<=100) {
    n = random(1,3);
  }
  if(valter<200 && valter>150) {
    n = random(2,4);
  }
  if(valter<=250 && valter>=200) {
    n = random(3,5); // (2,5)
  }
  if(valter>=300) {
   
    n = 4;
  }
  //Serial.print("\n");
  //Serial.print(n);
  //Serial.print("\n");
  switch(n)  {
    case 1 :
      colorSpread(random(8,10));
      break;
    case 2 :
      if (nPrev = 2)
        {
          colorSpread(random(8,10));
        }
      else{theaterChase(musicColor(random(50,400)),25,random(2,4));}
      break;
    case 3 :
      StrobeMiddle(random(5,7),25,30);
      break;
    case 4 :
      subEffectPicker("strobe");
      break;
    default :
      Serial.print("\n");
      Serial.print(n);
      Serial.print("\n");
      break;
  }

}

void subEffectPicker(String code, int intensity) {
  if(code="strobe") {  
    lPrev = l;
    l = random(1,5);
    if(l!=lPrev)  
      switch(l) {
        case 1 :
          if(intensity>=260) {
            l=3;
            break;
          }
          Strobe(random(3,7),25);
          break;
        case 2 :
          StrobeAll(180,180,180,10,50);
          break;
        case 3 :
          if(intensity>=280) {
            l=2;
            break;
          }
          brightPulse(100,random(7,15));
          break;
        case 4 :
          if(intensity>=280) {
            l=2;
            break;
          }
          //brightMusic(random(5,10));
          break;
                }
  }
}
void brightPulse(int beat, int count) {
  for(int i = count;i>=0;i--) {  
    int giga = readPin();
    if(giga>=beat) {  
      setAll(random(100,255),random(100,255),random(100,255));
      delay(100);
      pixels.setBrightness(brightness);
      showStrip();
      for(int i=brightness;i>0;i--) {
        pixels.setBrightness(i);
        showStrip();
        delay(2);
      }
    }
      
    }
    pixels.setBrightness(brightness);
    
}
void brightMusic(int runTime) {
  runTime = (runTime*1000)/25;
  for(int i=0;i<=runTime;i++)  {
    int giga = readPin();
    setAll(random(100,255),random(100,255),random(100,255));
    int gigad = constrain(giga,0,300);
    giga = map(gigad,0,300,0,brightness);
    pixels.setBrightness(giga);
    showStrip();
    delay(25);
  /*for(int i=giga;i>0;i--) {
    pixels.setBrightness(i);
    showStrip();
    delay(2);
  }
*/
  }
  pixels.setBrightness(brightness);
  
}
/*
First parameter is array name -- second is array size
Example:
int arr[] = {10, 324, 45, 90, 9808};
int n = sizeof(arr)/sizeof(arr[0]);
*/
int largest(int arr[], int n) {
    int i;
    int max = arr[0];
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
 
    return max;
}
void arrMake(int arr[], int count) {
  for(int i=0;i<count;i++) {
    arr[i] = readPin();
    delay(15);
  }
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
