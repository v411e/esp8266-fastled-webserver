uint8_t physicalToFibonacci[NUM_LEDS] = { 0, 13, 26, 39, 52, 57, 44, 31, 18, 5, 10, 23, 36, 49, 62, 54, 41, 28, 15, 2, 7, 20, 33, 46, 59, 51, 38, 25, 12, 4, 17, 30, 43, 56, 61, 48, 35, 22, 9, 1, 14, 27, 40, 53, 58, 45, 32, 19, 6, 11, 24, 37, 50, 63, 55, 42, 29, 16, 3, 8, 21, 34, 47, 60 };
uint8_t fibonacciToPhysical[NUM_LEDS] = { 0, 39, 19, 58, 29, 9, 48, 20, 59, 38, 10, 49, 28, 1, 40, 18, 57, 30, 8, 47, 21, 60, 37, 11, 50, 27, 2, 41, 17, 56, 31, 7, 46, 22, 61, 36, 12, 51, 26, 3, 42, 16, 55, 32, 6, 45, 23, 62, 35, 13, 52, 25, 4, 43, 15, 54, 33, 5, 44, 24, 63, 34, 14, 53 };
uint8_t coordsX[NUM_LEDS] = { 140, 189, 208, 214, 208, 146, 168, 180, 180, 162, 152, 146, 129, 103, 72, 40, 70, 97, 120, 131, 107, 79, 50, 23, 0, 7, 23, 46, 76, 93, 57, 37, 28, 29, 87, 68, 59, 62, 80, 113, 91, 94, 109, 133, 202, 172, 145, 125, 117, 145, 170, 198, 227, 253, 255, 235, 210, 181, 148, 175, 207, 228, 240, 244 };
uint8_t coordsY[NUM_LEDS] = { 128, 114, 91, 63, 34, 0, 21, 48, 76, 106, 78, 47, 25, 11, 5, 38, 35, 42, 61, 101, 87, 69, 68, 78, 98, 143, 118, 102, 98, 122, 131, 152, 179, 209, 255, 230, 202, 174, 148, 142, 181, 210, 235, 252, 235, 234, 224, 203, 170, 183, 201, 205, 198, 181, 134, 157, 171, 173, 153, 145, 138, 120, 93, 63 };
uint8_t angles[NUM_LEDS] = { 0, 249, 241, 232, 223, 200, 208, 217, 226, 235, 212, 203, 194, 185, 176, 162, 171, 180, 188, 197, 174, 165, 156, 147, 139, 124, 133, 142, 151, 136, 128, 119, 110, 101, 78, 86, 95, 104, 113, 99, 90, 81, 72, 63, 40, 49, 58, 67, 75, 52, 43, 34, 25, 17, 2, 11, 20, 29, 38, 14, 6, 255, 246, 237 };

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
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = angles[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void radiusPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t r = physicalToFibonacci[i];
    
    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (r * hues));
  }
}

void xPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void yPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (y * hues));
  }
}

void xyPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - ((x + y) * hues));
  }
}

void angleGradientPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = angles[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}

void radiusGradientPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t r = physicalToFibonacci[i];
    
    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (r * hues));
  }
}

void xGradientPalette() {
  uint8_t hues = 1;
  
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}

void yGradientPalette() {
  uint8_t hues = 1;
  
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (y * hues));
  }
}

void xyGradientPalette() {
  uint8_t hues = 1;
  
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

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

  const uint8_t hourRadius = 64;
  const uint8_t minuteRadius = 96;
  const uint8_t secondRadius = 255;

  const uint8_t handWidth = 64;

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
