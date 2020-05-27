unsigned long fadeint = 0;
int r = 0, g = 0, b = 0, rbit = 0;
boolean reversefade = 0;

void setup() {
  // put your setup code here, to run once:

fadeint = millis() + 100; // initialize fading interval
}

void loop() {
  // put your main code here, to run repeatedly:
if (millis() > fadeint) {
  colorfade();
  fadeint = millis() + 100; // Increment up by 100 millis
}

}

void colorfade() {
switch(weatherState) {
  case 0: // snow -- Starts at r: 200, g: 220, b: 255 -- ends at rgb: 255, 255, 255
    if ( r < 255 && ~reversefade) {
      /*increase RGB Value incrementally */
      r += 5;
      if (g < 255) {  // Green gets incremented up second
        g += 5;
      }
    } else {
      reversefade = 1;
    }
    if (reversefade && r > 200) {
      r -= 5;
      if (g > 220) {
        g -= 5;
      }
    } else {
      reversefade = 0;
    }

    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
  case 1: // clouds -- starts at rgb: 210, 200, 255 -- ends at rgb: 200, 220, 255
    if ( g < 220 && ~reversefade) {
      /*increase RGB Value incrementally */
      g += 2;
      if (r > 200) {  // Green gets incremented up second
        r -= 1;
      }
    } else {
      reversefade = 1;
    }
    if (reversefade && g > 200) {
      g -= 2;
      if (r < 210) {
        r += 1;
      }
    } else {
      reversefade = 0;
    }
    
    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
  case 2: // sun -- starts at rgb: 255, 0, 0 -- ends at rgb: 255, 255: 0
    if ( g < 255 && ~reversefade) {
      /*increase RGB Value incrementally */
      g += 20;
    } else {
      reversefade = 1;
    }
    if (reversefade && g > 0) {
      g -= 20;
    } else {
      reversefade = 0;
    }
    
    for (int i=pixStart; i < pixStart + LED_PER_SEG; i++){
      strip.setPixelColor(i, r, g, b);
    }
    strip.show();
    break;
  case 3: // rain -- starts at rgb: 0, 0, 255 -- ends at rgb: 0, 255, 255
    if ( g < 255 && ~reversefade) {
      /*increase RGB Value incrementally */
      g += 20;
    } else {
      reversefade = 1;
    }
    if (reversefade && g > 0) {
      g -= 20;
    } else {
      reversefade = 0;
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
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
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
