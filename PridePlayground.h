// Pride2015 by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.

uint8_t saturationBpm = 87;
uint8_t saturationMin = 220;
uint8_t saturationMax = 250;

uint8_t brightDepthBpm = 1;
uint8_t brightDepthMin = 96;
uint8_t brightDepthMax = 224;

uint8_t brightThetaIncBpm = 203;
uint8_t brightThetaIncMin = 25;
uint8_t brightThetaIncMax = 40;

uint8_t msMulitplierBpm = 147;
uint8_t msMulitplierMin = 23;
uint8_t msMulitplierMax = 60;

uint8_t hueIncBpm = 113;
uint8_t hueIncMin = 1;
uint8_t hueIncMax = 12;

uint8_t sHueBpm = 2;
uint8_t sHueMin = 5;
uint8_t sHueMax = 9;

void fillWithPridePlayground(bool useFibonacciOrder)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(saturationBpm, saturationMin, saturationMax);

  uint8_t brightdepth = beatsin88(brightDepthBpm * 256, brightDepthMin, brightDepthMax);
  uint16_t brightnessthetainc16 = beatsin88(brightThetaIncBpm, (brightThetaIncMin * 256), (brightThetaIncMax * 256));
  
  uint8_t msmultiplier = beatsin88(msMulitplierBpm, msMulitplierMin, msMulitplierMax);

  uint16_t hue16 = sHue16; //gHue * 256;
  uint16_t hueinc16 = beatsin88(hueIncBpm, hueIncMin, hueIncMax * 256);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(sHueBpm * 256, sHueMin, sHueMax);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;

    if (useFibonacciOrder) pixelnumber = fibonacciToPhysical[i];

    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void pridePlayground() {
  fillWithPridePlayground(false);
}

void pridePlaygroundFibonacci() {
  fillWithPridePlayground(true);
}
