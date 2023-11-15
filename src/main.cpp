/**

 */
#include "Arduino.h"
#include "Button2.h"
#include "HID-Project.h"

#ifdef NEOPIXEL
#define BUTTON_PIN 9
#define BUTTON_PWR 2

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 12;
const uint8_t PixelPin = 8;
const uint8_t AnimationChannels = 1;

#define colorSaturation 128
#define dimFactor 200

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor yellow(colorSaturation, colorSaturation, 0);
RgbColor purple(colorSaturation, 0, colorSaturation);
RgbColor cyan(0, colorSaturation, colorSaturation);
RgbColor black(0);
RgbColor actual_color(0);

uint16_t fast = 500;
uint16_t slow = 2000;
uint16_t actual_time = 0;

uint8_t actual_status = 1;

bool dim = false;

NeoPixelAnimator animations(AnimationChannels); // NeoPixel animation management object

boolean fadeToColor = false; // general purpose variable used to store effect state

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

// one entry per pixel to match the animation timing manager
MyAnimationState animationState[AnimationChannels];

#else
#include "Led.h"

#define BUTTON_PIN 2
#define RED_PIN    6
#define GREEN_PIN  9
#define BLUE_PIN   10
Led Led(RED_PIN, GREEN_PIN, BLUE_PIN );
#endif



Button2 button;
// These are automatically zero initialized
// This is our output buffer used by button presses
byte rawhidWriteBuffer[64];
// This is our input buffer used by RawHID
byte rawhidInBuffer[64];
// This is our 64b buffer to make sure we get 64b of input before processing
byte rawhidBuffer[64];
// Byte count to track input
int byte_count = 0;

void pressed(Button2 &btn) {
  rawhidWriteBuffer[3] = 0x04;  // Touch Button
  RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
  rawhidWriteBuffer[3] = 0x00;
  // #ifdef DEBUG
  // Serial.println("pressed");
  // #endif
}

void released(Button2 &btn) {
  rawhidWriteBuffer[3] = 0x02;  // Release Button
  RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
  rawhidWriteBuffer[3] = 0x00;
  // #ifdef DEBUG
  // Serial.print("released: ");
  // Serial.println(btn.wasPressedFor());
  // #endif
}

void longClickDetected(Button2 &btn) {
  rawhidWriteBuffer[3] = 0x01;  // Hold Button
  RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
  rawhidWriteBuffer[3] = 0x00;
#ifdef DEBUG
  Serial.println("long click detected\n");
#endif
}

void parseColor(int data) {
  switch (data) {
    case 0:
        actual_color = black;
        break;
    case 1:
        actual_color = red;
        break;
    case 2:
        actual_color = green;
        break;
    case 3:
        actual_color = yellow;
        break;
    case 4:
        actual_color = blue;
        break;
    case 5:
        actual_color = purple;
        break;
    case 6:
        actual_color = cyan;
        break;
    case 7:
        actual_color = white;
        break;
        // Don't set a default case, since there appear to be some undocumented values
        // sent fromt the software. We want to ignore those
    }
}
void parseEffect(int data) {
#ifdef DEBUG
  Serial.print("parseEffect: ");
  Serial.println(data);
#endif
  switch (data) {
    case 0:
      Serial.println("bright");
      actual_status = 1;
      break;
    case 1:
      Serial.println("dim");
      actual_status = 2;
      break;
    case 2:
      Serial.println("fast_pulse");
      actual_time = fast;
      actual_status = 3;
      break;
    case 3:
      Serial.println("slow_pulse");
      actual_status = 3;
      actual_time = slow;
      break;
      // Don't set a default case, since there appear to be some undocumented
      // values sent fromt the software. We want to ignore those
    }
}

// simple blend function
void BlendAnimUpdate(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
}

void FadeInFadeOutRinseRepeat(float luminance, RgbColor target, uint16_t time)
{
    if (fadeToColor)
    {
        // Fade upto a random color
        // we use HslColor object as it allows us to easily pick a hue
        // with the same saturation and luminance so the colors picked
        // will have similiar overall brightness
        // RgbColor target = HslColor(random(360) / 360.0f, 1.0f, luminance);
        // uint16_t time = random(800, 2000);

        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = target;

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }
    else
    {
        // fade to black
        uint16_t time = random(600, 700);

        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = RgbColor(0);

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }

    // toggle to the next effect state
    fadeToColor = !fadeToColor;
}

void setColor(RgbColor color, bool dimmer)
{
    RgbColor dimmed_color = color;
    if (dimmer)
    {
        dimmed_color = dimmed_color.Dim(dimFactor);
    }
    else
    {
        dimmed_color = dimmed_color.Brighten(dimFactor);
    }
    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, dimmed_color);
    }
}

void setup()
{
  // Led.setColor(LedColor::no_color);
  // Led.setEffect(LedEffect::bright);
  strip.Begin();
  setColor(red, true);
  strip.Show();
  delay(500);
  setColor(green, true);
  strip.Show();
  delay(500);
  setColor(blue, true);
  strip.Show();
  delay(500);

  // power the button
  digitalWrite(BUTTON_PWR, HIGH);

  button.begin(BUTTON_PIN);
  button.setPressedHandler(pressed);
  button.setLongClickDetectedHandler(longClickDetected);
  button.setReleasedHandler(released);

  // Workaround for bug when sending less than 64 bytes of data
  // Since arrays are zero init'd by default we shouldn't have to do this
  // anymore
  /* for (byte i = 0; i < sizeof(rawhidWriteBuffer); i++)
  {
      rawhidWriteBuffer[i] = 0x00;
  } */

  // There was a potential conflict before if you hit the button fast you could
  // read in Garbage data from HID because the buffer would get overwritten by
  // output
  RawHID.begin(rawhidInBuffer, sizeof(rawhidInBuffer));

#ifdef DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
  // Led.update();
  button.loop();
  int bytesAvailable = RawHID.available();
  if (bytesAvailable > 0) {
    while (bytesAvailable--) {
      rawhidBuffer[byte_count] = RawHID.read();
      byte_count++;
    }
    // We only care about the first byte.
    int hidData = rawhidBuffer[0];
    int ones = ((byte)hidData / 1) % 16;
    int tens = ((byte)hidData / 16) % 16;

    if ((ones >= 0 && ones <= 7) && (tens >= 0 && tens <= 3)) {
#ifdef DEBUG
      Serial.print("hidData: ");
      Serial.print(hidData, DEC);
      Serial.print(" / 0x");
      Serial.println(hidData, HEX);

      Serial.print("ones: ");
      Serial.print(ones, DEC);
      Serial.print(" / 0x");
      Serial.println(ones, HEX);

      Serial.print("tens: ");
      Serial.print(tens, DEC);
      Serial.print(" / 0x");
      Serial.println(tens, HEX);

      Serial.println();
#endif
      parseColor(ones);
      parseEffect(tens);
    } else {
#ifdef DEBUG
      Serial.print("Invalid/Undocumented data: ");
      Serial.print(ones);
      Serial.print(tens);
      Serial.print("/0x");
      Serial.println(hidData, HEX);
#endif
    }
#ifdef DEBUG
    for (byte i = 0; i < byte_count; i++) {
      Serial.print(rawhidBuffer[i], HEX);
      Serial.print(" ");
    }

    Serial.println();

    Serial.print("Byte Count: ");
    Serial.println(byte_count, DEC);
#endif
    // Reset our byte count once we have read the whole input buffer
    byte_count = 0;
  }

    if (actual_status == 3)
    {
        if (animations.IsAnimating())
        {
            // the normal loop just needs these two to run the active animations
            animations.UpdateAnimations();
            strip.Show();
        }
        else
        {
            // no animation runnning, start some
            //
            FadeInFadeOutRinseRepeat(0.25, actual_color, actual_time); // 0.0 = black, 0.25 is normal, 0.5 is bright
        }
    }
    else
    {
        if (actual_status == 2)
        {
            dim = true;
        }
        else
        {
            dim = false;
        }
        animations.StopAll();
        setColor(actual_color, dim);
        strip.Show();
    }
}