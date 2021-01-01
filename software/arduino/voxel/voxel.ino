// #define FASTLED_INTERNAL enable this to disable pragma message
#include <FastLED.h>
// venti 2020-12-06
// takes demo reel, modified to slow down and only use confetti
// add hardware fader support for settings

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    12
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    50 //50
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          80
//#define FRAMES_PER_SECOND  1 //120
//#define FADE_IN_DELAY 5
//#define FADE_OUT_DELAY 20

int FRAMES_PER_SECOND = 1;
int analog_pin_frames = A0;

int FADE_IN_DELAY = 5;
int FADE_OUT_DELAY_FACTOR = 2; //factor of fade_in_delay, e.g. FADE_IN_DELAY*4
int analog_pin_fade = A1;
int analog_fade = 500;

int debug_led_only = false;

void setup() {
  delay(3000); // 3 second delay for recovery, e.g. firmware flash  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);  
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gCurrentPatternNumber = 2; //confetti
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  if (FRAMES_PER_SECOND<1) {FRAMES_PER_SECOND=1;} //dont let them become 0 because of division 
  FastLED.delay(512/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 2000 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically

  //read fps
  int fps = analogRead(analog_pin_frames); //min 10 max 990
  if (fps<15) { fps=1; } // dont let them become 0 because of division
  if (fps>990) { fps=1024; }
  FRAMES_PER_SECOND = fps;
  Serial.print("fps: ");
  Serial.println(FRAMES_PER_SECOND);

  //read fade
  analog_fade = analogRead(analog_pin_fade); //min 10 max 990
  if (analog_fade<15) { analog_fade=1; }
  if (analog_fade>990) { analog_fade=1024; }
//  Serial.print("raw analog_fade: ");
//  Serial.println(analog_fade);  
  FADE_IN_DELAY = round(20*float(analog_fade)/1024);
  if (FADE_IN_DELAY<0) {FADE_IN_DELAY=0;}
  Serial.print("fade: ");
  Serial.println(FADE_IN_DELAY);  
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 1);
  int pos = random16(NUM_LEDS);
  if (debug_led_only) {
    pos = 5;
  }
  int new_hue = gHue + random8(64);
  if (random8(24)==0){
    new_hue = new_hue+128;
  }
  //read existing color (should be some faded pixel)
  uint8_t luma = leds[pos].getLuma();
  Serial.print("current luma: ");
  Serial.println(luma);
  //fade brightness to 0
  int fading=0;  
  luma=60;
  for (int b=0; b<luma; b++) {
    //leds[pos].fadeLightBy(5); //its amount of reduction on actual value of x/256
    leds[pos].nscale8_video(250);
    fading++;    
    FastLED.show(); 
    FastLED.delay(FADE_IN_DELAY*FADE_OUT_DELAY_FACTOR);
  }
  Serial.print("faded steps, waiting 250ms: ");
  Serial.println(fading);
  FastLED.delay(250);
  //fade brightness with new hue to 255
  int step_ = 1;
  luma = leds[pos].getLuma();
  Serial.print("current luma: ");
  Serial.println(luma);  
  for (int v=100; v<255-step_; v=v+step_) {
    leds[pos] = CHSV( new_hue, 200, v);
    FastLED.show(); 
    FastLED.delay(FADE_IN_DELAY);
  }
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
