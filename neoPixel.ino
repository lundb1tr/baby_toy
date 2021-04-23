#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
const int serialBaud = 9600;
const int forwardButtonPin = 2;
const int rgbStripPin = 3;
const int repeatButtonPin = 4;
const int forwardLedPin = 5;
const int reverseButtonPin = 7;
const int irPin = 11;
const int numberOfPixels = 120;
int oldForwardState = 1;
int oldRepeatState = 1;
int oldReverseState = 1;
int mode = 0;
int red = 255;
int green = 255;
int blue = 255;
int fakeDelay = 0;
bool increment = true;
IRrecv receiver = IRrecv(irPin);
Adafruit_NeoPixel forwardButtonLed = Adafruit_NeoPixel(1, forwardLedPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfPixels, rgbStripPin, NEO_GRB + NEO_KHZ800);
/* Should work to power 4 strips once purchased and wired up */
// Adafruit_NeoPixel stripArray[4] = [Adafruit_NeoPixel(numberOfPixels, rgbStripPin, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numberOfPixels, rgbStripPin, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numberOfPixels, rgbStripPin, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numberOfPixels, rgbStripPin, NEO_GRB + NEO_KHZ800)];
/* Should work to power 4 strips once purchased and wired up */
// const uint32_t lightOff = stripArray[0].Color(0, 0, 0);
const uint32_t lightOff = strip.Color(0, 0, 0);
const uint32_t theGoodStuff = strip.gamma32(strip.Color(255, 132, 108));

void setup() {
  Serial.begin(serialBaud);
  /* Should work to power 4 strips once purchased and wired up */
  // for (int stripNumber = stripArray.length - 1; stripNumber >= 0; stripNumber--) {
  //   stripArray[stripNumber].begin();
  //   for (int index = stripArray[stripNumber].numPixels() - 1; index >= 0; index--) {
  //     stripArray[stripNumber].setPixelColor(index, 128, 0, 255);
  //     stripArray[stripNumber].show();
  //     delay(20);
  //   }
  // }
  strip.begin();
  strip.setBrightness(25);
  for (int index = strip.numPixels() - 1; index >= 0; index--) {
    strip.setPixelColor(index, theGoodStuff);
    strip.show();
    delay(20);
  }
  // forwardButtonLed.begin();
  // forwardButtonLed.setPixelColor(0, red, green, blue);
  // forwardButtonLed.show();
  receiver.enableIRIn();
  pinMode(forwardButtonPin, INPUT_PULLUP);
  pinMode(repeatButtonPin, INPUT_PULLUP);
  pinMode(reverseButtonPin, INPUT_PULLUP);
}

void loop() {
  // if (fakeDelay == 20) {
  //   fakeDelay = 0;
  //   if (increment == true) {
  //     incrementButtonColors();
  //   } else {
  //     decrementButtonColors();
  //   }
  // } else {
  //   fakeDelay++;
  // }
  // forwardButtonLed.setPixelColor(0, red, green, blue);
  // forwardButtonLed.show();
  int newForwardState = digitalRead(forwardButtonPin);
  int newRepeatState = digitalRead(repeatButtonPin);
  int newReverseState = digitalRead(reverseButtonPin);
  bool goForward = ((newForwardState == 0) && (oldForwardState == 1));
  bool repeat = ((newRepeatState == 0) && (oldRepeatState == 1));
  bool reverse = ((newReverseState == 0) && (oldReverseState == 1));
  if (goForward) {
    delay(20);
    newForwardState = digitalRead(forwardButtonPin);
    if (newForwardState == 0) {
      if (++mode > 13) mode = 0;
      determineStripAction(mode);
    }
  } else if (receiver.decode()) {
    receiver.stop();
    if (receiver.decodedIRData.command != 0) {
      Serial.println(receiver.decodedIRData.command);
      mode = translateIrCommands(receiver.decodedIRData.command);
    }
    determineStripAction(mode);
    receiver.start();
  } else if (repeat) {
    delay(20);
    newRepeatState = digitalRead(repeatButtonPin);
    if (newRepeatState == 0) {
      determineStripAction(mode);
    }
  } else if (reverse) {
    delay(20);
    newReverseState = digitalRead(reverseButtonPin);
    if (newReverseState == 0) {
      if (--mode < 0) mode = 12;
      determineStripAction(mode);
    }
  }
  oldForwardState = newForwardState;
  oldRepeatState = newRepeatState;
  oldReverseState = newReverseState;
}

void incrementButtonColors() {
  if (green < 255 && red == 0) {
    green++;
  } else if (green == 255 && red < 255) {
    red++;
  } else {
    increment = false;
  }
}

void decrementButtonColors() {
  if (green == 255 && red > 0) {
    red--;
  } else if (red == 0 & green > 0) {
    green--;
  } else {
    increment = true;
  }
}

int translateIrCommands(int command) {
  switch(command) {
    case 17:
      return 0;
    case 4:
      return 1;
    case 5:
      return 2;
    case 6:
      return 3;
    case 8:
      return 4;
    case 9:
      return 5;
    case 10:
      return 6;
    case 12:
      return 7;
    case 13:
      return 8;
    case 14:
      return 9;
    case 35:
      return 10;
    case 19:
      return 11;
    case 15:
      return 12;
    default:
      return 2;
  }
}

void determineStripAction(int mode) {
  switch(mode) {
    case 0:
      reverseColorWipe(lightOff, 50);
      break;
    case 1:
      colorWipe(strip.Color(128, 0, 255), 50);
      break;
    case 2:
      reverseColorWipe(lightOff, 50);
      colorWipe(theGoodStuff, 50);
      break;
    case 3:
      reverseColorWipe(lightOff, 50);
      colorWipe(strip.Color(0, 0, 255), 50);
      break;
    case 4:
      reverseColorWipe(lightOff, 50);
      theaterChase(theGoodStuff, 50);
      break;
    case 5:
      reverseColorWipe(lightOff, 50);
      theaterChase(strip.Color(128, 0, 255), 50);
      break;
    case 6:
      reverseColorWipe(lightOff, 50);
      theaterChase(strip.Color(0, 0, 127), 50);
      break;
    case 7:
      reverseColorWipe(lightOff, 50);
      rainbow(10);
      break;
    case 8:
      reverseColorWipe(lightOff, 50);
      theaterChaseRainbow(50);
      break;
    case 9:
      reverseColorWipe(lightOff, 50);
      singleChaseSolidColor(theGoodStuff, 25);
      break;
    case 10:
      reverseColorWipe(lightOff, 25);
      singleChaseRandomColor(25);
      break;
    case 11:
      reverseColorWipe(lightOff, 25);
      backAndForth(theGoodStuff, 25);
      break;
    case 12:
      reverseColorWipe(lightOff, 50);
      inAndOut(theGoodStuff);
      break;
    default:
      reverseColorWipe(lightOff, 50);
      letsGetObnoxious(25);
  }
}

void letsGetObnoxious(int wait) {
  if (wait > 0) {
    randomColors(wait);
    reverseColorWipe(lightOff, wait);
    letsGetObnoxious(round(wait / 2));
  }
}

void randomColors(int wait) {
  for (int index = 0; index < strip.numPixels(); index++) {
    int red = random(0, 256);
    int green = random(0, 256);
    int blue = random(0, 256);
    uint32_t color = strip.Color(red, green, blue);
    strip.setPixelColor(index, color);
    strip.show();
    delay(wait);
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int index = 0; index < strip.numPixels(); index++) {
    strip.setPixelColor(index, color);
    strip.show();
    delay(wait);
  }
}

void reverseColorWipe(uint32_t color, int wait) {
  for (int index = strip.numPixels() - 1; index >= 0; index--) {
    strip.setPixelColor(index, color);
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 3; b++) {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);
      }
      strip.show();
      delay(wait);
    }  
  }
}

void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 3 * 65536; firstPixelHue += 256) {
    for (int index = 0; index < strip.numPixels(); index++) {
      int pixelHue = firstPixelHue + (index * 65536L / strip.numPixels());
      strip.setPixelColor(index, strip.gamma32(strip.ColorHSV(pixelHue)));
      forwardButtonLed.setPixelColor(0, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    forwardButtonLed.show();
    strip.show();
    delay(wait);
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;
  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 3; b++) {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
        int hue = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue));
        strip.setPixelColor(c, color);
      }
      strip.show();
      delay(wait);
      firstPixelHue += 65536 / 90;
    }
  }
}

void singleChaseSolidColor(uint32_t color, int wait) {
  for (int numberOfTries = 0; numberOfTries < 11; numberOfTries++) {
    for (int index = strip.numPixels() - 1; index >= 0; index--) {
      strip.setPixelColor(index, color);
      strip.show();
      delay(wait);
      strip.setPixelColor(index, lightOff);
    }
  }
}

void singleChaseRandomColor(int wait) {
  for (int numberOfTries = 0; numberOfTries <= 10; numberOfTries++) {
    for (int index = strip.numPixels() - 1; index >= 0; index--) {
      strip.setPixelColor(index, strip.gamma32(strip.Color(random(0, 256), random(0, 256), random(0, 256))));
      strip.show();
      delay(wait);
      strip.setPixelColor(index, lightOff);
    }
  }
}

void backAndForth(uint32_t color, int wait) {
  for (int numberOfTries = 0; numberOfTries <= 10; numberOfTries++) {
    for (int index = strip.numPixels() - 1; index >= 0; index--) {
      strip.setPixelColor(index, color);
      strip.show();
      delay(wait);
      strip.setPixelColor(index, lightOff);
    }
    for (int index = 0; index < strip.numPixels(); index++) {
      strip.setPixelColor(index, strip.gamma32(color));
      strip.show();
      delay(wait);
      strip.setPixelColor(index, lightOff);
    }
  }
}

void inAndOut(uint32_t color) {
  for (int numberOfTries = 0; numberOfTries <= 10; numberOfTries++) {
    for (int secondHalf = strip.numPixels() - 1; secondHalf > 14; secondHalf--) {
      int difference = strip.numPixels() - secondHalf;
      strip.setPixelColor(secondHalf, color);
      strip.setPixelColor(difference - 1, color);
      strip.show();
      delay(100);
      strip.setPixelColor(secondHalf, lightOff);
      strip.setPixelColor(difference -1, lightOff);
    }
    for (int secondHalf = 15; secondHalf <= strip.numPixels() - 1; secondHalf++) {
      int difference = strip.numPixels() - secondHalf;
      strip.setPixelColor(secondHalf, color);
      strip.setPixelColor(difference - 1, color);
      strip.show();
      delay(25);
      strip.setPixelColor(secondHalf, lightOff);
      strip.setPixelColor(difference - 1, lightOff);
    }
  }
}
