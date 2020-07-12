uint16_t physicalToFibonacci[NUM_LEDS] = { 0, 13, 26, 31, 18, 5, 10, 23, 28, 15, 2, 7, 20, 33, 25, 12, 4, 17, 30, 22, 9, 1, 14, 27, 32, 19, 6, 11, 24, 29, 16, 3, 8, 21 };
uint16_t fibonacciToPhysical[NUM_LEDS] = { 0, 21, 10, 31, 16, 5, 26, 11, 32, 20, 6, 27, 15, 1, 22, 9, 30, 17, 4, 25, 12, 33, 19, 7, 28, 14, 2, 23, 8, 29, 18, 3, 24, 13 };
uint8_t coordsX[NUM_LEDS]              = { 152, 111, 138, 163, 82, 184, 118, 102, 203, 63, 169, 160, 57, 224, 79, 121, 212, 29, 211, 129, 61, 250, 36, 161, 196, 13, 252, 83, 89, 255, 0, 210, 158, 19 };
uint8_t coordsY[NUM_LEDS]              = { 120, 140, 83, 156, 112, 90, 179, 62, 144, 149, 50, 198, 78, 101, 195, 27, 183, 125, 48, 226, 37, 135, 185, 7, 223, 84, 69, 236, 0, 181, 154, 7, 255, 35 };
uint8_t angles[NUM_LEDS]               = { 255, 94, 193, 32, 132, 232, 70, 170, 9, 108, 208, 47, 147, 246, 85, 185, 23, 123, 223, 62, 161, 0, 100, 199, 38, 138, 237, 76, 176, 15, 114, 214, 53, 152 };

void drawSpiralLine(uint8_t angle, int step, CRGB color)
{
  int startIndex = 0;
  int smallestAngleDifference = 255;

  // find the outermost led closest to the desired angle
  for (int i = 0; i < NUM_LEDS; i++) {
    int j = physicalToFibonacci[i];
    if (j < step) continue;
    if (!(j + step >= NUM_LEDS)) continue; // not outermost
    uint8_t a = angles[i];
    if (a == angle) startIndex = i;
    else if (angle - a > 0 && angle - a < smallestAngleDifference) {
      smallestAngleDifference = angle - a;
      startIndex = i;
    }
  }

  // draw the starting LED
  leds[startIndex] += color;

  // draw to center from outer start
  int f = physicalToFibonacci[startIndex];
  while (f - step >= 0 && f - step < NUM_LEDS) {
    leds[fibonacciToPhysical[f]] += color;
    f = f - step;
  }
}

void setPixelAR(uint8_t angle, uint8_t dAngle, uint8_t radius, uint8_t dRadius, CRGB color)
{
  uint16_t amax = qadd8(angle, dAngle);
  uint8_t amin = qsub8(angle, dAngle);

  uint16_t rmax = qadd8(radius, dRadius);
  uint16_t rmin = qsub8(radius, dRadius);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t o = i;

    uint8_t ao = angles[o];

    if (ao <= amax && ao >= amin) {
      uint8_t ro = physicalToFibonacci[o];

      if (ro <= rmax && ro >= rmin) {
        leds[i] = color;
      }
    }
  }
}

void andPixelAR(uint8_t angle, uint8_t dAngle, uint8_t startRadius, uint8_t endRadius, CRGB color)
{
  uint16_t amax = qadd8(angle, dAngle);
  uint8_t amin = qsub8(angle, dAngle);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t o = i;

    uint8_t ao = angles[o];

    if (ao <= amax && ao >= amin) {
      uint8_t ro = physicalToFibonacci[o];

      if (ro <= endRadius && ro >= startRadius) {
        leds[i] += color;
      }
    }
  }
}

void antialiasPixelAR(uint8_t angle, uint8_t dAngle, uint8_t startRadius, uint8_t endRadius, CRGB color)
{
  uint16_t amax = qadd8(angle, dAngle);
  uint8_t amin = qsub8(angle, dAngle);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t o = i;

    uint8_t ao = angles[o];

    uint8_t adiff = qsub8(max(ao, angle), min(ao, angle));
    uint8_t fade = qmul8(adiff, 32);
    CRGB faded = color;
    faded.fadeToBlackBy(fade);

    if (ao <= amax && ao >= amin) {
      uint8_t ro = physicalToFibonacci[o];

      if (ro <= endRadius && ro >= startRadius) {
        leds[i] += faded;
      }
    }
  }
}

void anglePalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = angles[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void radiusPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t r = physicalToFibonacci[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (r * hues));
  }
}

void xPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = coordsX[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void yPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (y * hues));
  }
}

void xyPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - ((x + y) * hues));
  }
}

void angleGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = angles[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}

void radiusGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t r = physicalToFibonacci[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (r * hues));
  }
}

void xGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = coordsX[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}

void yGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (y * hues));
  }
}

void xyGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - ((x + y) * hues));
  }
}

void drawAnalogClock() {
  float second = timeClient.getSeconds();
  float minute = timeClient.getMinutes() + (second / 60.0);
  float hour = timeClient.getHours() + (minute / 60.0);

  static uint8_t hourAngle = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  const uint8_t hourRadius = 96;
  const uint8_t minuteRadius = 192;
  const uint8_t secondRadius = 255;

  const uint8_t handWidth = 32;

  const float degreesPerSecond = 255.0 / 60.0;
  const float degreesPerMinute = 255.0 / 60.0;
  const float degreesPerHour = 255.0 / 12.0;

  EVERY_N_MILLIS(100) {
    hourAngle = 255 - hour * degreesPerHour;
    minuteAngle = 255 - minute * degreesPerMinute;
    secondAngle = 255 - second * degreesPerSecond;
  }

  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);

  antialiasPixelAR(secondAngle, handWidth, 0, secondRadius, CRGB::Blue);
  antialiasPixelAR(minuteAngle, handWidth, 0, minuteRadius, CRGB::Green);
  antialiasPixelAR(hourAngle, handWidth, 0, hourRadius, CRGB::Red);
  leds[0] = CRGB::Red;
}

void drawSpiralAnalogClock(uint8_t step) {
  float second = timeClient.getSeconds();
  float minute = timeClient.getMinutes() + (second / 60.0);
  float hour = timeClient.getHours() + (minute / 60.0);

  static uint8_t hourAngle = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  const float degreesPerSecond = 255.0 / 60.0;
  const float degreesPerMinute = 255.0 / 60.0;
  const float degreesPerHour = 255.0 / 12.0;

  EVERY_N_MILLIS(100) {
    hourAngle = 255 - hour * degreesPerHour;
    minuteAngle = 255 - minute * degreesPerMinute;
    secondAngle = 255 - second * degreesPerSecond;
  }

  drawSpiralLine(secondAngle, step, CRGB(0, 0, 2));
  drawSpiralLine(minuteAngle, step, CRGB(0, 2, 0));
  drawSpiralLine(hourAngle, step, CRGB(2, 0, 0));
}

void drawSpiralAnalogClock13() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(13);
}

void drawSpiralAnalogClock21() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(21);
}

void drawSpiralAnalogClock34() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(34);
}

void drawSpiralAnalogClock55() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(55);
}

void drawSpiralAnalogClock89() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(89);
}

void drawSpiralAnalogClock21and34() {
  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);
  drawSpiralAnalogClock(21);
  drawSpiralAnalogClock(34);
}

void drawSpiralAnalogClock13_21_and_34() {
  float second = timeClient.getSeconds();
  float minute = timeClient.getMinutes() + (second / 60.0);
  float hour = timeClient.getHours() + (minute / 60.0);

  static uint8_t hourAngle = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  const float degreesPerSecond = 255.0 / 60.0;
  const float degreesPerMinute = 255.0 / 60.0;
  const float degreesPerHour = 255.0 / 12.0;

  EVERY_N_MILLIS(100) {
    hourAngle = 255 - hour * degreesPerHour;
    minuteAngle = 255 - minute * degreesPerMinute;
    secondAngle = 255 - second * degreesPerSecond;
  }

  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);

  drawSpiralLine(secondAngle, 13, CRGB(0, 0, 2));
  drawSpiralLine(minuteAngle, 21, CRGB(0, 2, 0));
  drawSpiralLine(hourAngle, 34, CRGB(2, 0, 0));
}

void drawSpiralAnalogClock34_21_and_13() {
  float second = timeClient.getSeconds();
  float minute = timeClient.getMinutes() + (second / 60.0);
  float hour = timeClient.getHours() + (minute / 60.0);

  static uint8_t hourAngle = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  const float degreesPerSecond = 255.0 / 60.0;
  const float degreesPerMinute = 255.0 / 60.0;
  const float degreesPerHour = 255.0 / 12.0;

  EVERY_N_MILLIS(100) {
    hourAngle = 255 - hour * degreesPerHour;
    minuteAngle = 255 - minute * degreesPerMinute;
    secondAngle = 255 - second * degreesPerSecond;
  }

  fadeToBlackBy(leds, NUM_LEDS, clockBackgroundFade);

  drawSpiralLine(secondAngle, 34, CRGB(0, 0, 2));
  drawSpiralLine(minuteAngle, 21, CRGB(0, 2, 0));
  drawSpiralLine(hourAngle, 13, CRGB(2, 0, 0));
}
