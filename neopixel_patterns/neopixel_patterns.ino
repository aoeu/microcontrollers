#include <Adafruit_NeoPixel.h>

enum pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
enum direction { FORWARD, REVERSE };

class NeoPatterns : public Adafruit_NeoPixel {
public:

pattern ActivePattern;
direction Direction;

unsigned long Interval;
unsigned long lastUpdate;

uint32_t Color1, Color2;
uint16_t TotalSteps;
uint16_t Index;

void (*OnComplete)();

NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)()) :Adafruit_NeoPixel(pixels, pin, type) {
	OnComplete = callback;
}

void Update() {
	if ((millis() - lastUpdate) > Interval) {
		lastUpdate = millis();
		switch(ActivePattern) {
		case RAINBOW_CYCLE:
			RainbowCycleUpdate();
			break;
		case THEATER_CHASE:
			TheaterChaseUpdate();
			break;
		case COLOR_WIPE:
			ColorWipeUpdate();
			break;
		case SCANNER:
			ScannerUpdate();
			break;
		case FADE:
			FadeUpdate();
			break;
		default:
			break;
		}
	}
}

void Increment() {
	if (Direction == FORWARD) {
		Index++;
		if (Index >= TotalSteps) {
			Index = 0;
			if (OnComplete != NULL) {
				OnComplete();
			}
		}
	} else { // Direction == REVERSE
		--Index;
		if (Index <= 0) {
			Index = TotalSteps - 1;
			if (OnComplete != NULL) {
				OnComplete();
			}
		}
	}
}

void RainbowCycle(uint8_t interval, direction dir = FORWARD) {
	ActivePattern = RAINBOW_CYCLE;
	Interval = interval;
	TotalSteps = 255;
	Index = 0;
	Direction = dir;
}

void RainbowCycleUpdate() {
	for (int i = 0; i < numPixels(); i++) {
		setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
		show();
		Increment();
	}
}

void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD) {
	ActivePattern = COLOR_WIPE;
	Interval = interval;
	TotalSteps = numPixels();
	Color1 = color;
	Index = 0;
	Direction = dir;
}

void ColorWipeUpdate() {
	setPixelColor(Index, Color1);
	show();
	Increment();
}

void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD) {
	ActivePattern = THEATER_CHASE;
	Interval = interval;
	TotalSteps = numPixels();
	Color1 = color1;
	Color2 = color2;
	Index = 0;
	Direction = dir;
}

void TheaterChaseUpdate() {
	for (int i=0; i < numPixels(); i++) {
		if ((i + Index) % 3 == 0) {
			setPixelColor(i, Color1);
		} else {
			setPixelColor(i, Color2);
		}
	}
	show();
	Increment();
}

void Scanner(uint32_t color1, uint8_t interval) {
	ActivePattern = SCANNER;
	Interval = interval;
	TotalSteps = (numPixels() - 1) * 2;
	Color1 = color1;
	Index = 0;
}

void ScannerUpdate() {
	for (int i = 0; i < numPixels(); i++) {
		if (i == Index) {
			Serial.print(i);
			setPixelColor(i, Color1);
		} else if (i == TotalSteps - Index) {
			Serial.print(i);
			setPixelColor(i, Color1);
		} else {
			setPixelColor(i, DimColor(getPixelColor(i)));
		}
	}
	show();
	Increment();
}

void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD) {
	ActivePattern = FADE;
	Interval = interval;
	TotalSteps = steps;
	Color1 = color1;
	Color2 = color2;
	Index = 0;
	Direction = dir;
}

void FadeUpdate() {
	uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
	uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
	uint8_t blue = ((Blue(Color1) * (TotalSteps -Index)) + (Blue(Color2) * Index)) / TotalSteps;
	ColorSet(Color(red, green, blue));
	show();
	Increment();
}

uint8_t Red(uint32_t color) {
	return (color >> 16) & 0xFF;
}

uint8_t Green(uint32_t color) {
	return (color >> 8) & 0xFF;
}

uint8_t Blue(uint32_t color) {
	return color & 0xFF;
}

uint32_t DimColor(uint32_t color) {
	uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
	return dimColor;
}

uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} else if (WheelPos < 170) {
		WheelPos -= 85;
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	} else {
		WheelPos -= 170;
		return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}

void Reverse() {
	if (Direction == FORWARD) {
		Direction = REVERSE;
		Index = TotalSteps - 1;
	} else {
		Direction = FORWARD;
		Index = 0;
	}
}

void ColorSet(uint32_t color) {
	for (int i = 0; i < numPixels(); i++) {
		setPixelColor(i, color);
	}
	show();
}

};

#define PIN 7

void RingComplete();
NeoPatterns Ring(60, PIN, NEO_GRB + NEO_KHZ800, &RingComplete);

uint8_t numStepsPerFade = 64;
uint8_t millisecondsPerStep = 200;
uint32_t red = Ring.Color(255, 0, 0);
uint32_t blue = Ring.Color(0, 0, 255);
uint32_t green = Ring.Color(0, 255, 0);

void setup() {
	Ring.begin();
	Ring.Fade(red, blue, numStepsPerFade, millisecondsPerStep, FORWARD);
}

void loop() {
	Ring.Update();
}

void RingComplete() {
	if (Ring.Color1 == red && Ring.Color2 == blue) {
		Ring.Fade(blue, green, numStepsPerFade, millisecondsPerStep, FORWARD);
	} else if (Ring.Color1 == blue && Ring.Color2 == green ) {
		Ring.Fade(green, red, numStepsPerFade, millisecondsPerStep, FORWARD);
	} else if (Ring.Color1 == green && Ring.Color2 == red ) {
		Ring.Fade(red, blue, numStepsPerFade, millisecondsPerStep, FORWARD);
	}
}
