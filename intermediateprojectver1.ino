// 9/21/2023 | CRT 101 A
// Intermediate Project PinUp: Function

#include <Adafruit_NeoPixel.h>

#define buttonPin 2 // pin for Button
#define pxPin 6 // pin for NeoPixels
#define pxCount 14 // pixel count

int           pixelCycle = 0;           // Pattern Pixel Cycle
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelInterval = 50;       // Pixel Interval (ms)
uint16_t      pixelNumber = pxCount;  // Total Number of Pixels

Adafruit_NeoPixel strip(pxCount, pxPin); // pixelCount, pin

boolean currentReading = false;
boolean prevReading = false;
int state = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLUP);
  strip.begin(); // initialize strip
  // strip.setBrightness(50);
  strip.show();
  // start(buttonPin, neoPin)
}

void loop() {
 prevReading = currentReading;
  currentReading = debounce(buttonPin, prevReading); // debounces button
  if (currentReading == HIGH && prevReading == LOW) {
    state++;
    if (state > 6) {
      state = 0;
    }
  }

  switch(state) {
    case 0: // off
      Serial.println("in case 0");
      colorWipe(strip.Color(0,0,0), 10); 
      break;
    case 1: // gold
      Serial.println("in case 1");
      colorWipe(strip.Color(255,215,0), 10);
      break;
     case 2: // bouncingBrightness, orange
      Serial.println("in case 2");
      bouncingBrightness(strip.Color(255, 165, 0), 20);
      break;
    case 3: // triColorSweep
      Serial.println("in case 3");
      triColorSweep(strip.Color(255, 192, 203), strip.Color(255, 0, 255), strip.Color(0, 0, 255), 500);
      break;
    case 4: // dark orange
      Serial.println("in case 4");
      colorWipe(strip.Color(255,140,0), 10);
      break;
    case 5: // blood orange
      Serial.println("in case 5");
      colorWipe(strip.Color(255,15,0), 10);
      break;
    case 6: // rainbow chase
      Serial.println("in case 6");
      theaterChaseRainbow(50);
      break;
  }
}

boolean debounce(int aPin, boolean aPrevState) {
  boolean aButtonReading = digitalRead(aPin);

  if (aButtonReading == HIGH && aPrevState == LOW) {
    delay(15);
  }
  return aButtonReading;
}

// taken from strandtest (example library)
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// taken from strandtest (example library)
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// taken from strandtest (example library)
// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(uint8_t wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   
  for(uint16_t i=0; i < pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time  
  }
  strip.show();                             //  Update strip to match
  pixelCycle++;                             //  Advance current cycle
  if(pixelCycle >= 256)
    pixelCycle = 0;                         //  Loop the cycle back to the begining
}

// taken from strandtest (example library)
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   //  Update delay time  
  for(int i=0; i < pixelNumber; i+=3) {
    strip.setPixelColor(i + pixelQueue, Wheel((i + pixelCycle) % 255)); //  Update delay time  
  }
  strip.show();
  for(int i=0; i < pixelNumber; i+=3) {
    strip.setPixelColor(i + pixelQueue, strip.Color(0, 0, 0)); //  Update delay time  
  }      
  pixelQueue++;                           //  Advance current queue  
  pixelCycle++;                           //  Advance current cycle
  if(pixelQueue >= 3)
    pixelQueue = 0;                       //  Loop
  if(pixelCycle >= 256)
    pixelCycle = 0;                       //  Loop
}

void loopingLight(uint32_t aColor, int waitTime) {
  static int index = 0;
  static unsigned long startTime = millis();

  strip.setPixelColor(index, aColor);
  strip.show();

  // make timer
  if (millis() - startTime >= waitTime) {
    strip.setPixelColor(index, strip.Color(0, 0, 0));  // turn pixel off
    index++;
    startTime = millis();

    if (index >= strip.numPixels()) {
      index = 0;
    }
  }
}

void bouncingLight(uint32_t aColor, int waitTime) {
  static int index = 0;
  static unsigned long startTime = millis();
  static boolean isInc = true;

  // while isInc is true -> timer will make light travel forward on strip
  // while isInc is false -> light travel backwards

  strip.setPixelColor(index, aColor);
  strip.show();
  if (millis() - startTime >= waitTime) {
    if (isInc == true) {
      strip.setPixelColor(index, strip.Color(0, 0, 0));
      index++;
      startTime = millis();
    }

    if (isInc == false) {
      strip.setPixelColor(index, strip.Color(0, 0, 0));
      index--;
      startTime = millis();
    }

    if (index >= strip.numPixels()) {
      isInc = false;
    }

    if (index == 0) {
      isInc = true;
    }
  }
}

void setStripColor(uint32_t aColor) {
  // index of start, how far, how much
  for (int i = 0; i < strip.numPixels(); i++) { //i += 2 -> goes every other one
    strip.setPixelColor(i, aColor);
  }
  strip.show();
}

void bouncingBrightness (uint32_t aColor, int waitTime) {
  static int brightness = 0;
  static unsigned long startTime = millis();
  static boolean isInc = true;

  // while isInc is true -> timer will make light travel forward on strip
  // while isInc is false -> light travel backwards

  // strip.setPixelColor(index, aColor);
  strip.show();
  if (millis() - startTime >= waitTime) {
    if (isInc == true) {
      setStripColor(aColor);
      brightness++;
      strip.setBrightness(brightness);
      startTime = millis();
    }

    if (isInc == false) {
      setStripColor(aColor);
      brightness--;
      strip.setBrightness(brightness);
      startTime = millis();
    }

    if (brightness >= 250) {
      isInc = false;
    }

    if (brightness <= 5) {
      isInc = true;
    }
  }
}

void triColor (uint32_t aColor, uint32_t aColor2, uint32_t aColor3) {
  for (int i = 0; i < strip.numPixels(); i++) {
    if (i % 3 == 0) {
      strip.setPixelColor(i, aColor);
    }
    if (i % 3 == 1) { // 3 alternating colors would be i%2 == 2)
      strip.setPixelColor(i, aColor2);
    }
    if (i % 3 == 2) {
      strip.setPixelColor(i, aColor3);
    }
  }
  strip.show();
}

void triColorSweep (uint32_t aColor, uint32_t aColor2, uint32_t aColor3, int waitTime) {
static unsigned long startTime = millis();
static int state = 0;
static int index = 0;

// strip.setPixelColor(index, aColor);

if (millis() - startTime >= waitTime) {
  state++;
  state %= 3;
  startTime = millis();
  if (state == 0) {
    triColor(aColor, aColor2, aColor3);
  }
  if (state == 1) {
    triColor(aColor2, aColor3, aColor);
  }
  if (state == 2) {
    triColor(aColor3, aColor, aColor2);
  }
  strip.show();
}
if (index >= strip.numPixels()) {
  index = 0;
}
}




