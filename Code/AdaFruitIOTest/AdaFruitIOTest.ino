#include "AdafruitIO.h"
#include "Adafruit_MQTT.h"


#include "config.h"

// before running this code, create feeds on Adafruit IO that match these names:
AdafruitIO_Feed *Temp = io.feed("Temp"); // set up the 'hightemp' feed

void setup() {

  // start the serial connection
  Serial.begin(115200);


  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  Temp->onMessage(handleTemp);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

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
  Serial.println(todaysHigh);
  delay(500);                     // wait half a second
}  
