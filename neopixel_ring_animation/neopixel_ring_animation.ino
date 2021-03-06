// Source code taken from https://raw.githubusercontent.com/adafruit/Adafruit_NeoPixel/master/examples/strandtest_wheel/strandtest_wheel.ino

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 0

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint32_t ledBrightness = 10;
uint8_t numPixels = 16;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.setBrightness(ledBrightness);
  strip.show(); // Initialize all pixels to 'off'
}

uint32_t brightRed = strip.Color(255, 0, 0);
uint32_t brightGreen = strip.Color(0, 255, 0);
uint32_t brightBlue = strip.Color(0, 0, 255);
uint32_t white = strip.Color(127, 127, 127);
uint32_t red = strip.Color(127, 0, 0);
uint32_t blue = strip.Color(0, 0, 127);

uint32_t orange = strip.Color(255, 64, 0);
uint32_t yellow = strip.Color(255, 127, 0);

void loop() {
  // Some example procedures showing how to display to the pixels:
  // colorWipe(brightRed, 50);
  // colorWipe(brightGreen, 50);
  // colorWipe(brightBlue, 50);

  // Send a theater pixel chase in...
  // theaterChase(white, 50);
  // theaterChase(red, 50);
  // theaterChase(blue, 50);

  // rainbow(20);
  // rainbowCycle(20);
  // theaterChaseRainbow(50);

  // flickerFlameColors();
  // pinwheel();
  tricolorWheel();
}

void flickerFlameColors() {
	colorWipe(brightRed, randFreq());
	colorWipe(orange, randFreq());
	colorWipe(yellow, randFreq());
}

void pinwheel() {
  uint8_t i, colorIndex, pixelIndex;
  uint32_t colors[] = { brightRed, brightRed, brightRed, brightRed,
			blue, blue, blue, blue,
			yellow, yellow, yellow, yellow,
			brightGreen, brightGreen, brightGreen, brightGreen };
  for(i = 0; i < 16; i++) {
    for (pixelIndex = 0; pixelIndex < numPixels; pixelIndex++) {
      colorIndex = pixelIndex + i;
      if (colorIndex >= numPixels) colorIndex -= numPixels;
      strip.setPixelColor(pixelIndex, colors[colorIndex]);
    }
    strip.show();
    delay(200);
  }
}

void tricolorWheel() {
  uint32_t colors[] = {
    brightRed, brightRed, brightRed, brightRed, brightRed, brightRed, brightRed, brightRed,
    blue, blue, blue, blue, blue, blue, blue, blue,
    yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow,
  };
  uint8_t i, colorIndex, pixelIndex, colorsLen = 24;
  for (i = 0; i < colorsLen; i++) {
    for (pixelIndex = 0; pixelIndex < numPixels; pixelIndex++) {
      colorIndex = pixelIndex + i;
      if (colorIndex >= colorsLen) colorIndex -= colorsLen;
      strip.setPixelColor(pixelIndex, colors[colorIndex]);
    }
    strip.show();
    delay(200);
  }
}

uint8_t randFreq() {
  return (uint8_t)(random(1, 10) * 10);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i < (numPixels ) + 1; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<numPixels; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< numPixels; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / numPixels) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < numPixels; i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < numPixels; i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < numPixels; i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < numPixels; i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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

