#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Wire.h> 
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include <LiquidCrystal.h>
#include "config.h"

#define PIN 5
#define NUM_LEDS 24
#define LED_PER_SEG 6
#define CYCLE_INT (1 * 1000 * 60UL)  // Intervals of 2 minute

#define TFT_CS     21
#define TFT_RST    22
#define TFT_DC     19
#define TFT_SCLK   23
#define TFT_MOSI   18


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


// LCD Initialize
const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

AdafruitIO_Feed *hightemp = io.feed("hightemp"); // set up the 'hightemp' feed
AdafruitIO_Feed *precipitation = io.feed("precipitation"); // set up the 'precipitation' feed
AdafruitIO_Feed *hourlytime = io.feed("time"); // set up the 'hourlytime' feed

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int weatherState;
uint16_t pixStart = 0;
unsigned long fadeint = 0;
int r = 0, g = 0, b = 0, rbit = 0;
boolean reversefade = 0;
static unsigned long patternInterval = 0;  // initialize such that a reading is due the first time through loop()
int minutes = 0;
int hourlyInt = 0;
unsigned long oneMinuteCounter = 0;
int brightness = 200;


void setup() {
  Serial.begin(115200);
  strip.setBrightness(brightness);
  strip.begin();
  
  strip.show();
  strip.clear();  // Initialize all pixels to 'off'

  Serial.print("Connecting to Adafruit IO");
  io.connect();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

// Starting TFT
tft.initR(INITR_BLACKTAB);
tft.fillScreen(ST77XX_BLACK);
screenSetup();


  weatherState = 4;

  hightemp->onMessage(handleTemp);
  precipitation->onMessage(handleCondition);
  hourlytime->onMessage(handleTime);
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    colorfade();
    delay(500);
  }

  colorWipe(strip.Color(255,255,255,255),100);
  colorWipe(strip.Color(0,0,255,0),100);
  Serial.println();
  Serial.println(io.statusText());

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Getting Temp...");
  lcd.setCursor(0, 1);
  lcd.print("Getting Cond...");
  delay(3000);
  
  fadeint = millis() + 100; // initialize fading interval
  oneMinuteCounter = millis() + 60 * 1000;
  patternInterval = millis() + CYCLE_INT;

}

void loop() {

// Run IO Program
  io.run();

if (millis() > oneMinuteCounter) {
  ++minutes;
  Serial.print(hourlyInt);
  Serial.print(':');
  Serial.print(minutes);
  Serial.print('\n');
  drawClock();
  oneMinuteCounter = millis() + (60 * 1000);
}

if ((millis() + 11) > patternInterval) {
      whiteOverRainbow(1,75,5,3);
      Serial.println("pattern");
      strip.clear();
      patternInterval += CYCLE_INT;
}

  // put your main code here, to run repeatedly:
if (millis() > fadeint) {
  colorfade();
  fadeint = millis() + 100; // Increment up by 100 millis
}

}

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

  drawThermometer(todaysHigh);
  delay(500);                     // wait half a second
}

void handleCondition(AdafruitIO_Data *data) {
  strip.clear();
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
  }

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
}

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

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
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

      if(loopNum == loops) {
        return;
      }
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
      io.run();
      strip.show();
    }
  }
  
}

void handleTime(AdafruitIO_Data *data) {
  minutes = 0;
  String checkTime = data->toString();
  String hourlyTime = checkTime.substring(checkTime.lastIndexOf(' ')+1,checkTime.lastIndexOf(' ')+3);
  hourlyInt = hourlyTime.toInt();

  // Dim lights during night time
  char ampm = checkTime.charAt(checkTime.length() - 2);
  if ((ampm == 'P' && hourlyInt > 9) || (ampm == 'A' && hourlyInt < 5)) {
    brightness = 50;
    strip.setBrightness(brightness);
  } else {
    brightness = 200;
    strip.setBrightness(brightness);
  }
  Serial.println(hourlyInt);
  drawClock();
  oneMinuteCounter = millis() + 60 * 1000;
}

void screenSetup() {
  // Text wrap option
  tft.setTextWrap(false);
 
  // Initializes the screen to Black
  tft.fillScreen(ST77XX_BLACK);
 
  // The Weather Men Title
  tft.setCursor(5, 15);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("WeatherMen");
 
  // The clock box
  tft.drawRoundRect(5, 45, 120, 43, 5, ST77XX_WHITE);
}

void drawClock() {
 
  // Writes Refreshed Clock to screen
  tft.fillRect(7, 47, 116, 39, ST77XX_BLACK);
  tft.setCursor(5, 52);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);
  if(hourlyInt < 10)
    tft.print('0');
  tft.print(hourlyInt);
  tft.print(':');
  if(minutes < 10)
    tft.print('0');
  tft.print(minutes);
}

void drawThermometer(int temp) {
  // Thermometer Reading calcultater
  int thermoReading = 100 * temp / 120;
 
  // Themometer Shapes
  tft.drawRoundRect(15, 105, 110, 10, 5, ST77XX_WHITE);
  tft.fillCircle(15, 110, 7, ST77XX_BLUE);
  tft.drawCircle(15, 110, 8, ST77XX_WHITE);
  tft.fillRect(15, 106, 100, 8, ST77XX_BLACK);
  tft.fillRect(15, 106, thermoReading, 8, ST77XX_BLUE);
 
  // Thermometer Scale
  tft.setCursor(22, 97);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("0   40   80   120");
}
