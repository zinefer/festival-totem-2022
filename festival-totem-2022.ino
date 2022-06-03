// Platform
#include "esp_system.h"
#include "driver/periph_ctrl.h"
#include <analogWrite.h>

// LED Deps
#define FASTLED_INTERNAL
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#include "src/config.h"

CRGB leds[NUM_LEDS];
uint8_t brightness = 50;
uint8_t ringBrightness = 64;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

uint8_t ledanim  = 0;
uint8_t ringanim = 0;
uint8_t laseranim = 0;

void setup()
{
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  Serial.write("help");

  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode (LASERA, OUTPUT);
  pinMode (LASERB, OUTPUT);
  pinMode (LASERC, OUTPUT);
  digitalWrite(LASERA, LOW);
  digitalWrite(LASERB, LOW);
  digitalWrite(LASERC, LOW);

  analogWriteResolution(RINGA, 10);
  analogWriteResolution(RINGB, 10);
  analogWriteResolution(RINGC, 10);
  analogWriteResolution(RINGD, 10);
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER,DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
}

void loop()
{
  Serial.write("ahhh");
  switch (ledanim) {
    case 0:
      led_rainbow();
    break;
    case 1:
      led_rainbow();
      led_glitter(80);
    break;
    case 2:
      led_pulse(220);
    break;
    default: ledanim=0; break;
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  switch (ringanim) {
    case 0:
      ring_pulse();
    break;
    case 1:
      ring_chase(false);
    break;
    case 2:
      ring_chase(true);
     break;
    default: ringanim=0; break;
  }

  switch (laseranim) {
    case 0:
      laser_pulse();
    break;
    default: laseranim=0; break;
  }
  
  EVERY_N_MILLISECONDS( 10 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS( 30000 ) { laseranim++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS( 30000 ) { ringanim++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS( 30000 ) { ledanim++; }
}

// LED PATTERNS
void led_rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void led_glitter(fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void led_pulse(uint8_t hue) {
  /*0,

  1-5

  6-10
 ----
  11,

  12-16

  17-21*/
  
  uint8_t BeatsPerMinute = 30;

  Serial.write("hdhjkdhjkdhjksdhjk");
  uint8_t beatA = beatsin8( BeatsPerMinute, 1, 255);
  uint8_t beatB = beatsin8( BeatsPerMinute, 1, 255);
  uint8_t beatC = beatsin8( BeatsPerMinute, 1, 255);

  beatB+=33;
  beatC+=66;

  leds[0].setHSV(hue, beatA, brightness);

  for (int i = 1; i<=5; i++) {
    leds[i].setHSV(hue, beatB, brightness);
  }

  for (int i = 6; i<=10; i++) {
    leds[i].setHSV(hue, beatC, brightness);
  }
  
  leds[11].setHSV(hue, beatA, brightness);

  for (int i = 12; i<=16; i++) {
    leds[i].setHSV(hue, beatB, brightness);
  }

  for (int i = 17; i<=21; i++) {
    leds[i].setHSV(hue, beatC, brightness);
  }
}

// RING PATTERNS
void ring_pulse()
{
  uint8_t BeatsPerMinute = 30;
  uint8_t beat = beatsin8( BeatsPerMinute, 1, ringBrightness);
  analogWrite(RINGA, beat);
  analogWrite(RINGB, beat);
  analogWrite(RINGC, beat);
  analogWrite(RINGD, beat);
}

void ring_chase(bool invert)
{
  uint8_t BeatsPerMinute = 16;
  uint8_t beat = beatsin8( BeatsPerMinute, 0, 3);

  uint8_t low = 0;
  uint8_t high = ringBrightness;
  if (invert) {
    low = ringBrightness;
    high = 0;
  }
  
  switch(beat) {
    case 0:
      analogWrite(RINGA, high);
      analogWrite(RINGB, low);
      analogWrite(RINGC, low);
      analogWrite(RINGD, low);
    break;
    case 1:
      analogWrite(RINGA, low);
      analogWrite(RINGB, high);
      analogWrite(RINGC, low);
      analogWrite(RINGD, low);
    break;
    case 2:
      analogWrite(RINGA, low);
      analogWrite(RINGB, low);
      analogWrite(RINGC, high);
      analogWrite(RINGD, low);
    break;
    case 3:
      analogWrite(RINGA, low);
      analogWrite(RINGB, low);
      analogWrite(RINGC, low);
      analogWrite(RINGD, high);
    break;
  } 
}

// LASER PATTERNS
void laser_pulse()
{
  /*uint8_t BeatsPerMinute = 4;
  uint8_t beat = beatsin8( BeatsPerMinute, 0, 3);
  switch (beat) {
    case 0:
      digitalWrite(LASERA, HIGH);
      digitalWrite(LASERB, LOW);
      digitalWrite(LASERC, LOW);
    break;
    case 1:
      digitalWrite(LASERA, LOW);
      digitalWrite(LASERB, HIGH);
      digitalWrite(LASERC, LOW);
    break;
    case 2:
      digitalWrite(LASERA, LOW);
      digitalWrite(LASERB, LOW);
      digitalWrite(LASERC, HIGH);
    break;
  }*/
}
