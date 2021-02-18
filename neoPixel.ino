#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
const int serialBaud = 9600;
const int buttonPin = 2;
const int numberOfPixels = 30;
const int stripPin = 3;
const int irPin = 11;
int oldState = 1;
int mode = 0;
IRrecv receiver = IRrecv(irPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfPixels, stripPin, NEO_GRB + NEO_KHZ800);
const uint32_t lightOff = strip.Color(0, 0, 0);

void setup() {
  Serial.begin(serialBaud);
  strip.begin();
  for (int index = strip.numPixels() - 1; index >= 0; index--) {
    strip.setPixelColor(index, 128, 0, 255);
    strip.show();
    delay(20);
  }
  receiver.enableIRIn();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int newState = digitalRead(buttonPin);
  if ((newState == 0) && (oldState == 1)) {
    delay(20);
    newState = digitalRead(buttonPin);
    if (newState == 0) {
      if (++mode > 8) mode = 0;
      determineStripAction(mode);
    }
  } else {
    if (receiver.decode()) {
      receiver.stop();
      Serial.println(receiver.decodedIRData.command);
      if (receiver.decodedIRData.command != 0) {
        mode = translateIrCommands(receiver.decodedIRData.command);
      }
      determineStripAction(mode);
      receiver.start();
    }
  }
  oldState = newState;
}

int translateIrCommands(int command) {
  switch(command) {
    case 22:
      return 0;
    case 12:
      return 1;
    case 24:
      return 2;
    case 94:
      return 3;
    case 8:
      return 4;
    case 28:
      return 5;
    case 90:
      return 6;
    case 66:
      return 7;
    case 82:
      return 8;
    case 74:
      return 9;
    case 25:
      return 10;
    case 13:
      return 11;
    default:
      return 12;
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
      colorWipe(strip.Color(0, 255, 0), 50);
      break;
    case 3:
      reverseColorWipe(lightOff, 50);
      colorWipe(strip.Color(0, 0, 255), 50);
      break;
    case 4:
      reverseColorWipe(lightOff, 50);
      theaterChase(strip.Color(127, 127, 127), 50);
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
      singleChaseSolidColor(strip.Color(random(0, 256), random(0, 256), random(0, 256)), 25);
      break;
    case 10:
      reverseColorWipe(lightOff, 25);
      singleChaseRandomColor(25);
      break;
    case 11:
      reverseColorWipe(lightOff, 25);
      backAndForth(strip.Color(random(0, 256), random(0, 256), random(0, 256)), 25);
      break;
    case 12:
      reverseColorWipe(lightOff, 50);
      letsGetObnoxious(200);
      break;
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
    }
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
      strip.setPixelColor(index, strip.Color(random(0, 256), random(0, 256), random(0, 256)));
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
