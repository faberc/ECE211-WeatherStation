#include <Adafruit_NeoPixel.h>

#define PIN 32
#define NUM_LEDS 28
#define BRIGHTNESS 50
#define LED_PER_SEG 7


#include <Wire.h> 
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include "config.h"

AdafruitIO_Feed *precipitation = io.feed("weather-station-prototype-1.precipitation"); // set up the 'precipitation' feed

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int weatherState;
uint16_t pixStart = 0;
unsigned long fadeint = 0;
int r = 0, g = 0, b = 0, rbit = 0;
boolean reversefade = 0;

void setup() {
  Serial.begin(115200);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  strip.clear();  // Initialize all pixels to 'off'

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  precipitation->onMessage(handleCondition);
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());


  weatherState = 4;
  delay(3000);
  fadeint = millis() + 100; // initialize fading interval
  
}

void loop() {

// Run IO Program
  io.run();

  // put your main code here, to run repeatedly:
if (millis() > fadeint) {
  colorfade();
  fadeint = millis() + 100; // Increment up by 100 millis
}

}

void handleCondition(AdafruitIO_Data *data) {
  strip.clear();
  String forecast = data->toString(); // store the incoming weather data in a string
 
 
  //the following strings store the varous IFTTT weather report words I've discovered so far
  String rain = String("Rain");
  String lightrain = String("Light Rain");
  String rainshower = String ("Rain Shower");
  String snow = String("Snow");
  String cloudy = String("Cloudy");
  String mostlycloudy = String("Mostly Cloudy");
  String partlycloudy = String("Partly Cloudy");
  String clearsky = String("Clear");
  String fair = String("Fair");
  String sunny = String("Sunny");
  String rainandsnow = String("Rain and Snow");
  String snowshower = String("Snow Shower");
 
  // These if statements compare the incoming weather variable to the stored conditions, and control the NeoPixels accordingly.

  // if there's snow in the forecast, tell the the first four pixels to be whiteish blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    Serial.println("snow in the forecast today");
    weatherState = 0;
    pixStart = 0;
    r = 200;
    g = 220;
    b = 255;
    for (int i = pixStart; i < pixStart+LED_PER_SEG; ++i){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }

  // if there're clouds in the forecast, tell the the middle four pixels to white and the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    Serial.println("cloudy sky in the forecast today");
    weatherState = 1;
    pixStart = LED_PER_SEG;
    r = 210;
    g = 200;
    b = 255;
    for (int i = pixStart; i < pixStart+LED_PER_SEG; ++i){
      strip.setPixelColor(i, r, g, b);
    }
   strip.show(); // light up the pixels
}

  // if there's sun in the forecast, tell the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    Serial.println("some kind of sun in the forecast today");
    weatherState = 2;
    pixStart = LED_PER_SEG*2;
    r = 255;
    g = 0;
    b = 0;
    for (int i = pixStart; i < pixStart+LED_PER_SEG; ++i){
      strip.setPixelColor(i, r, g, b);
  }
}
 
  // if there's rain in the forecast, tell the the first four pixels to be blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(rain) || forecast.equalsIgnoreCase(lightrain) || forecast.equalsIgnoreCase(rainshower)){
    Serial.println("rain in the forecast today");
    weatherState = 3;
    pixStart = LED_PER_SEG*3;
    r = 0;
    g = 0;
    b = 255;
    for (int i = pixStart; i < pixStart+LED_PER_SEG; ++i){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
  }
}

void colorfade() {
switch(weatherState) {
  case 0: // snow -- Starts at r: 200, g: 220, b: 255 -- ends at rgb: 255, 255, 255
    if (~reversefade) {
      if (r > 255) {
        reversefade = 1;
      }
      r += 5;
      if (g < 255) {  // Green gets incremented up second
        g += 5;
      }
    } else {
      if (r < 200) {
        reversefade = 0;
      }
      r -= 5;
      if (g > 220) {
        g -= 5;
      }

    }

    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
    
  case 1: // clouds -- starts at rgb: 210, 200, 255 -- ends at rgb: 200, 220, 255
    if (~reversefade) {
      if (g > 220) {
        reversefade = 1;
      }
      /*increase RGB Value incrementally */
      g += 2;
      if (r > 200) {  // Green gets incremented up second
        r -= 1;
      }
    } else {
      if (g < 200) {
      reversefade = 0;
    }
      g -= 2;
      if (r < 210) {
        r += 1;
      }
    }
    
    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
    
  case 2: // sun -- starts at rgb: 255, 0, 0 -- ends at rgb: 255, 255: 0
    if (~reversefade) {
      if ( g > 255) {
        reversefade = 1;
      }
      g += 20;
    } else {
    if (g < 0) {
      reversefade = 0;
    }
      g -= 20;
    }
    
    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
    
  case 3: // rain -- starts at rgb: 0, 0, 255 -- ends at rgb: 0, 255, 255
    if (~reversefade) {
      if ( g > 255) {
        reversefade = 1;
      }
      g += 20;
    } else {
    if (g < 0) {
      reversefade = 0;
    }
      g -= 20;
    }
    
    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
    
  default:
  if (rbit < 256) {
    ++rbit;
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+rbit) & 255));
    }
    strip.show();
  } else {
    rbit = 0;
  }
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
