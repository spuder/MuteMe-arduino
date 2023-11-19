// # include <Arduino.h>
# include <LedNeoPixel.h>
// # include <Led.h>
// # include <NeoPixelBus.h>
// # include <NeoPixelAnimator.h>

boolean fadeToColor = false;
const uint16_t colorSaturation=128;
const uint16_t dimFactor = 200;

const uint16_t PixelCount = 12;
const uint8_t PixelPin = 8;
const uint8_t AnimationChannels = 1;

uint16_t fast = 500;
uint16_t slow = 2000;
uint16_t actual_time = 0;

RgbColor actual_color(0);

NeoPixelAnimator animations(AnimationChannels);

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

MyAnimationState animationState[AnimationChannels];

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

void LedNeoPixel::initStrip()
{
    strip.Begin();
    strip.Show();
}

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

void LedNeoPixel::FadeInFadeOutRinseRepeat(float luminance, RgbColor target, uint16_t time)
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

void LedNeoPixel::setColor(LedColor color)
{
    switch (color) {
        case 0:
            actual_color = (0);
            break;
        case 1:
            actual_color = RgbColor(colorSaturation, 0, 0);
            break;
        case 2:
            actual_color = RgbColor(0, colorSaturation, 0);
            break;
        case 3:
            actual_color = RgbColor(colorSaturation, colorSaturation, 0);
            break;
        case 4:
            actual_color = RgbColor(0, 0, colorSaturation);
            break;
        case 5:
            actual_color = RgbColor(colorSaturation, 0, colorSaturation);
            break;
        case 6:
            actual_color = RgbColor(0, colorSaturation, colorSaturation);
            break;
        case 7:
            actual_color = RgbColor(colorSaturation);
            break;
    }
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, actual_color);
    }
}

void setColorRGB(RgbColor color)
{
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, color);
    }
}

void LedNeoPixel::setEffect(LedEffect effect) {
    this->effect = effect;
}

void LedNeoPixel::brighten(RgbColor color) {
    RgbColor dimmed_color = color.Dim(dimFactor);
    // dimmed_color = dimmed_color.Dim(dimFactor);
    setColorRGB(dimmed_color);
}

void LedNeoPixel::darken(RgbColor color) {
    RgbColor dimmed_color = color.Brighten(dimFactor);
    // dimmed_color = dimmed_color.Brighten(dimFactor);
    setColorRGB(dimmed_color);
}

void LedNeoPixel::AnimationControl(RgbColor color, uint16_t time) {
    if (animations.IsAnimating())
    {
        animations.UpdateAnimations();
        strip.Show();
    }
    else
    {
        FadeInFadeOutRinseRepeat(0.25, color, time);
    }
}

void LedNeoPixel::update() {
    switch (this->effect) {
        case LedEffect::bright:
            animations.StopAll();
            brighten(strip.GetPixelColor(0));
            strip.Show();
            break;
        case LedEffect::dim:
            animations.StopAll();
            darken(strip.GetPixelColor(0));
            strip.Show();
            break;
        case LedEffect::fast_pulse:
            actual_time = fast;
            AnimationControl(strip.GetPixelColor(0), actual_time);
            break;
        case LedEffect::slow_pulse:
            actual_time = slow;
            AnimationControl(strip.GetPixelColor(0), actual_time);
            break;
    }
}