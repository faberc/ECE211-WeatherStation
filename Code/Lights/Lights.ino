#include <Adafruit_NeoPixel.h>

#define PIN 32
#define NUM_LEDS 28
#define BRIGHTNESS 50
#define LED_PER_SEG 7


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// FIXME: Just for testing different colors
int weatherState = 0;

// Initialize all colors to 0
int rgbw_init[4] = {0, 0, 0, 0};
int rgbw_fin[4] = {0, 0, 0, 0};

uint16_t pixStart;


byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {

switch (weatherState) {
  case 0:   // Rain
    pixStart = 0;
    int rgbval[] = {0, 0, 255, 0};
    memcpy(rgbw_init, &rgbval, 4);
    rgbw_fin[4] = {0, 255, 255, 255};
    int rgbval[] = {0, 255, 255, 255};
    memcpy(rgbw_fin, &rgbval, 4);
    break;
  case 1:   // Sun
    pixStart = LED_PER_SEG;
    rgbw_init[4] = {255, 0, 0, 0};
    rgbw_fin[4] = {255, 255, 0, 0};
    break;
  case 2:   // Cloudy
    pixStart = LED_PER_SEG*2;
    rgbw_init[4] = {255, 0, 255, 255};
    rgbw_fin[4] = {0, 0, 255, 0};
    break;
  case 3:   // Snow
    pixStart = LED_PER_SEG*3;
    rgbw_init[4] = {0, 0, 0, 255};
    rgbw_fin[4] = {0, 255, 255, 255};
    break;
}

for(uint16_t i=pixStart; i<(pixStart+LED_PER_SEG); i++) {
    strip.setPixelColor(i, strip.Color(rgbw_init[0],rgbw_init[1],rgbw_init[2],rgbw_init[3]));
    strip.show();
    delay(5);
  }


//clearPixels();
//
//if (weatherState == 1) {
//
//r = 255;
//
//for(uint16_t i=pixStart; i<(pix_Start+LED_PER_SEG); i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(5);
//  }
//
//while(g<255) {
//   g = g + 1;
//  for(uint16_t i=0; i<7; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//
//while (g>0) {
//   g = g - 1;
//  for(uint16_t i=0; i<7; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//}
//
//else if (weatherState = 2) {
//g = 0;
//b = 255;
//r = 0;
//
//for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(5);
//  }
//
//while(r<204 && g < 229) {
//   r = r + 1;
//   g = g + 1;
//  for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//
//while (r > 0 && g > 0) {
//   g = g - 1;
//   r = r - 1;
//  for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//}
//
//else if (weatherState = 3) {
//g = 0;
//b = 255;
//r = 0;
//
//for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//
//while(r<204 && g < 229) {
//   r = r + 1;
//   g = g + 1;
//  for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//
//while (r > 0 && g > 0) {
//   g = g - 1;
//   r = r - 1;
//  for(uint16_t i=7; i<14; i++) {
//    strip.setPixelColor(i, strip.Color(r, g, b, w));
//    strip.show();
//    delay(1);
//  }
//}
//}
//

}

void clearPixels() {
  colorWipe(strip.Color(0,0,0,0), 1);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
