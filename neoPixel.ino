#include <Adafruit_NeoPixel.h>
const int serialBaud = 9600;
const int buttonPin = 2;
const int numberOfPixels = 30;
const int stripPin = 3;
boolean oldState = HIGH;
int mode = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfPixels, stripPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(serialBaud);
  strip.begin();
  strip.show();
  pinMode(buttonPin, INPUT_PULLUP);  
}

void loop() {
  int newState = digitalRead(buttonPin);
  if ((newState == 0) && (oldState == 1)) {
    delay(20);
    newState = digitalRead(buttonPin);
    if (newState == 0) {
      if (++mode > 8) mode = 0;
      switch(mode) {
        case 0:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          break;
        case 1:
          colorWipe(strip.Color(255, 0, 0), 50);
          break;
        case 2:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          colorWipe(strip.Color(0, 255, 0), 50);
          break;
        case 3:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          colorWipe(strip.Color(0, 0, 255), 50);
          break;
        case 4:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          theaterChase(strip.Color(127, 127, 127), 50);
          break;
        case 5:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          theaterChase(strip.Color(127, 0, 0), 50);
          break;
        case 6:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          theaterChase(strip.Color(0, 0, 127), 50);
          break;
        case 7:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          rainbow(10);
          break;
        case 8:
          reverseColorWipe(strip.Color(0, 0, 0), 50);
          theaterChaseRainbow(50);
          break;
      }
    }
  }
  oldState = newState;
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
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
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
