
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <Adafruit_GFX.h>
#include "Adafruit_NeoPixel.h"
 
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
 
#include <LiquidCrystal.h>
 
 
// LCD Initialize
const int rs = 13, en = 12, d4 = 27, d5 = 33, d6 = 15, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
 
// Adafruit IO Subscription Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
 
// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
 
// Configuration you can optionally change (but probably want to keep the same):
#define PIXEL_PIN       14                      // Pin connected to the NeoPixel data input.
#define PIXEL_COUNT     24                      // Number of NeoPixels.
#define PIXEL_TYPE      NEO_GRBW + NEO_KHZ800   // Type of the NeoPixels (see strandtest example).
 
// before running this code, create feeds on Adafruit IO that match these names:
AdafruitIO_Feed *hightemp = io.feed("weather-station-prototype-1.hightemp"); // set up the 'hightemp' feed
AdafruitIO_Feed *precipitation = io.feed("weather-station-prototype-1.precipitation"); // set up the 'precipitation' feed
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE); // create NeoPixels object
 
void setup() {
   
  // start the serial connection
  Serial.begin(115200);
 
  // Initialize NeoPixels and turn them off.
  pixels.begin();
  lightPixels(pixels.Color(0, 0, 0, 0));
 
  Serial.print("Connecting to Adafruit IO");
 
  // connect to io.adafruit.com
  io.connect();
 
  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  hightemp->onMessage(handleTemp);
  precipitation->onMessage(handleCondition);
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
 
  lcd.begin(16, 2);
  lcd.print("Getting Temp...");
  lcd.setCursor(0, 1);
  lcd.print("Getting Cond...");
  delay(3000);
 
}
 
void loop() {
 
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}
 
// this function is called whenever a feed message
// is received from Adafruit IO. it was attached to
// the feed in the setup() function above.
void handleTemp(AdafruitIO_Data *data) {
 
  Serial.print("received <- ");
  Serial.println(data->value());  // print the temperature data to the serial monitor
 
  int todaysHigh = data->toInt(); // store the incoming temperature data as an integer
 
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(todaysHigh);
  lcd.print(" F");
 
  delay(500);                     // wait half a second
}  
 
void handleCondition(AdafruitIO_Data *data) {
 
  lightPixels(pixels.Color(0, 0, 0, 0)); // reset all pixels to off
 
  String forecast = data->toString(); // store the incoming weather data in a string
 
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(forecast);
 
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
 
  // if there's rain in the forecast, tell the the first four pixels to be blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(rain) || forecast.equalsIgnoreCase(lightrain) || forecast.equalsIgnoreCase(rainshower)){
    Serial.println("rain in the forecast today");
    pixels.setPixelColor(0, pixels.Color(0, 30, 200, 20));
    pixels.setPixelColor(1, pixels.Color(0, 30, 200, 20));
    pixels.setPixelColor(2, pixels.Color(0, 30, 200, 20));
    pixels.setPixelColor(3, pixels.Color(0, 30, 200, 20));
    pixels.setPixelColor(4, pixels.Color(0, 0, 255, 0));
    pixels.setPixelColor(5, pixels.Color(0, 0, 255, 0));
  }
 
  // if there's snow in the forecast, tell the the first four pixels to be whiteish blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    Serial.println("snow in the forecast today");
    pixels.setPixelColor(6, pixels.Color(0, 0, 0, 255));
    pixels.setPixelColor(7, pixels.Color(0, 0, 0, 255));
    pixels.setPixelColor(8, pixels.Color(0, 0, 0, 255));
    pixels.setPixelColor(9, pixels.Color(0, 0, 0, 255));
    pixels.setPixelColor(10, pixels.Color(0, 0, 0, 255));
    pixels.setPixelColor(11, pixels.Color(0, 0, 0, 255));
  }
 
  // if there's sun in the forecast, tell the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    Serial.println("some kind of sun in the forecast today");
    pixels.setPixelColor(12, pixels.Color(255, 150, 0, 0));
    pixels.setPixelColor(13, pixels.Color(255, 150, 0, 0));
    pixels.setPixelColor(14, pixels.Color(255, 150, 0, 0));
    pixels.setPixelColor(15, pixels.Color(255, 150, 0, 0));
    pixels.setPixelColor(16, pixels.Color(255, 150, 0, 0));
    pixels.setPixelColor(17, pixels.Color(255, 150, 0, 0));
  }
  // if there're clouds in the forecast, tell the the middle four pixels to white and the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    Serial.println("cloudy sky in the forecast today");
    pixels.setPixelColor(18, pixels.Color(128, 0, 128, 0));
    pixels.setPixelColor(19, pixels.Color(128, 0, 128, 0));
    pixels.setPixelColor(20, pixels.Color(128, 0, 128, 0));
    pixels.setPixelColor(21, pixels.Color(128, 0, 128, 0));
    pixels.setPixelColor(22, pixels.Color(128, 0, 128, 0));
    pixels.setPixelColor(23, pixels.Color(255, 0, 255, 0));
   }
   pixels.show(); // light up the pixels
}
 
// Function to set all the NeoPixels to the specified color.
void lightPixels(uint32_t color) {
  for (int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
