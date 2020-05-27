#include <Adafruit_NeoPixel.h>

#define PIN 32
#define NUM_LEDS 28
#define BRIGHTNESS 50
#define LED_PER_SEG 7
#define CYCLE_INT (1 * 60UL)  // Intervals of 1 minute

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


// FIXME: Just for testing different colors
int weatherState = 2;
uint16_t pixStart = 0;
static unsigned long patternInterval = CYCLE_INT;  // initialize such that a reading is due the first time through loop()

void setup() {
  Serial.begin(115200);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  strip.clear();  // Initialize all pixels to 'off'
}


void loop() {

if ((millis() / 1000) >= patternInterval) {
      whiteOverRainbow(20,75,5,3);
      Serial.println("pattern");
      strip.clear();
      patternInterval += CYCLE_INT;
      
}
  
switch (weatherState) {
  case 0: // Snow
    pixStart = 0;
    colorWipe(strip.Color(255, 255 ,255),130);
    colorWipe(strip.Color(105, 105 ,105),130);
    colorWipe(strip.Color(128, 128 ,128),130);
    colorWipe(strip.Color(169, 169 ,169),130);
    break;
  case 1:  // Cloudy
    pixStart = LED_PER_SEG;
    colorWipe(strip.Color(75,0,130),130);
    colorWipe(strip.Color(153,50,204),130);
    colorWipe(strip.Color(255,0,255),130);
    colorWipe(strip.Color(102,0,102),130);
    break;
  case 2: // Sun
    pixStart = LED_PER_SEG*2;
    colorWipe(strip.Color(255,165,0),130);
    colorWipe(strip.Color(255,69,0),130);
    colorWipe(strip.Color(255,0,0),130);
    colorWipe(strip.Color(255,69,0),130);
    break;
  case 3: // Rain
    pixStart = LED_PER_SEG*3;
    colorWipe(strip.Color(0,0,139),130);
    colorWipe(strip.Color(0,0,255),130);
    colorWipe(strip.Color(30,144,255),130);
    colorWipe(strip.Color(135,206,250),130);
    break;
  default:
    rainbow(20);
    break;
}
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=pixStart; i<pixStart+LED_PER_SEG; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
 
 
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


void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength, uint8_t loops) {
  
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
        else{
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
  
}
