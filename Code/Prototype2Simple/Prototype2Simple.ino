#include <Adafruit_NeoPixel.h>

#define PIN 32
#define NUM_LEDS 24
#define BRIGHTNESS 50
#define LED_PER_SEG 6
#define CYCLE_INT (1 * 60UL)  // Intervals of 1 minute


#include <Wire.h> 
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include <LiquidCrystal.h>
#include "config.h"


// LCD Initialize
const int rs = 13, en = 12, d4 = 27, d5 = 33, d6 = 15, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

AdafruitIO_Feed *hightemp = io.feed("weather-station-prototype-1.hightemp"); // set up the 'hightemp' feed
AdafruitIO_Feed *precipitation = io.feed("weather-station-prototype-1.precipitation"); // set up the 'precipitation' feed

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int weatherState;
int breakloop = 0;
uint16_t pixStart = 0;
static unsigned long patternInterval = CYCLE_INT;  // initialize such that a reading is due the first time through loop()

void setup() {
  Serial.begin(115200);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  strip.clear();  // Initialize all pixels to 'off'

  Serial.print("Connecting to Adafruit IO");
  io.connect();
  lcd.begin(16, 2);
  lcd.print("Connecting...");

  hightemp->onMessage(handleTemp);
  precipitation->onMessage(handleCondition);
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    rainbow(20);
    delay(500);
  }

  Serial.println();
  whiteOverRainbow(1,75,5,1);
  Serial.println(io.statusText());

  lcd.begin(16, 2);
  lcd.print("Getting Temp...");
  lcd.setCursor(0, 1);
  lcd.print("Getting Cond...");
  weatherState = 4;
  delay(3000);
  
}

void loop() {

// Run IO Program
  io.run();

if ((millis() / 1000) >= patternInterval) {
      whiteOverRainbow(1,75,5,3);
      Serial.println("pattern");
      strip.clear();
      patternInterval += CYCLE_INT;
      returnToState();
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
 
  // These if statements compare the incoming weather variable to the stored conditions, and control the NeoPixels accordingly.
 
  // if there's rain in the forecast, tell the the first four pixels to be blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(rain) || forecast.equalsIgnoreCase(lightrain) || forecast.equalsIgnoreCase(rainshower)){
    Serial.println("rain in the forecast today");
    weatherState = 3;
    pixStart = LED_PER_SEG*3;
    colorWipe(strip.Color(0,0,255),10);
  }
 
  // if there's snow in the forecast, tell the the first four pixels to be whiteish blue and the middle four pixels to be white (but don't draw them yet)
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    Serial.println("snow in the forecast today");
    weatherState = 0;
    pixStart = 0;
    colorWipe(strip.Color(255, 255 ,255),10);
  }
 
  // if there's sun in the forecast, tell the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    Serial.println("some kind of sun in the forecast today");
    weatherState = 2;
    pixStart = LED_PER_SEG*2;
    colorWipe(strip.Color(255,165,0),10);
  }
  // if there're clouds in the forecast, tell the the middle four pixels to white and the last four pixels to be yellow (but don't draw them yet)
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    Serial.println("cloudy sky in the forecast today");
    weatherState = 1;
    pixStart = LED_PER_SEG;
    colorWipe(strip.Color(153,50,204),10);
   }
   strip.show(); // light up the pixels
   breakloop = 1;
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

      if(loopNum == loops || breakloop) {
        breakloop = 0; 
        return;
      }
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
      strip.show();
      io.run();
    }
  }
  
}

void readDelay(int ms){
  for(int i = 0; i < ms; i++){
    io.run();
    delay(1);
    if (breakloop) {
      breakloop = 0;
      return;
    }
  }
}

void returnToState () {
  strip.clear(); 
  // if there's snow in the forecast, tell the the first four pixels to be whiteish blue and the middle four pixels to be white (but don't draw them yet)
  if (weatherState == 0){
    pixStart = 0;
    colorWipe(strip.Color(255, 255 ,255),10);
  }
   // if there're clouds in the forecast, tell the the middle four pixels to white and the last four pixels to be yellow (but don't draw them yet)
  if (weatherState == 1){
    pixStart = LED_PER_SEG;
    colorWipe(strip.Color(153,50,204),10);
   }
  // if there's sun in the forecast, tell the last four pixels to be yellow (but don't draw them yet)
  if (weatherState == 2){
    pixStart = LED_PER_SEG*2;
    colorWipe(strip.Color(255,165,0),10);
  }
  // if there's rain in the forecast, tell the the first four pixels to be blue and the middle four pixels to be white (but don't draw them yet)
  if (weatherState == 3){
    pixStart = LED_PER_SEG*3;
    colorWipe(strip.Color(0,0,255),10);
  }
   strip.show(); // light up the pixels
}
