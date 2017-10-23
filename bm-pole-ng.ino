#include <NXPMotionSense.h>
#include <Wire.h>
//#include <EEPROM.h>
#include "FastLED.h"
FASTLED_USING_NAMESPACE

NXPMotionSense imu;
NXPSensorFusion filter;

#define DATA_PIN      8
//#define CLK_PIN       13
#define LED_TYPE      WS2812B
#define COLOR_ORDER   BGR
#define NUM_LEDS      96
CRGB leds[NUM_LEDS];
#define BRIGHTNESS    255
#define FRAMES_PER_SECOND  200
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
uint8_t gHue = 20; // rotating "base color" used by many of the patterns
uint8_t gSat = 255;
uint8_t pos = 72;

void setup() {
  imu.begin();
  filter.begin(100);

//  pinMode(7, OUTPUT); 
//  digitalWrite(7, HIGH);
  // tell FastLED about the LED strip configuration
//  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER,DATA_RATE_MHZ(12)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
 
  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

   gHue = abs(2*filter.getYaw()*255/360);
   gSat = 255-abs(filter.getRoll()*255/360);
   pos = abs((filter.getPitch() * NUM_LEDS / 180) - (NUM_LEDS/2)); 
   fadeToBlackBy( leds, NUM_LEDS, 8);
   leds[pos] += CHSV( gHue, gSat, 255);
   FastLED.show();
  }
}
