# include <LedNeoPixel.h>

boolean fadeToColor = false;
const uint16_t colorSaturation=128;
const uint16_t dimFactor = 200;

const uint16_t PixelCount = 12;
const uint8_t PixelPin = 8;
const uint8_t AnimationChannels = 1;

uint16_t fast = 500;
uint16_t slow = 2500;
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

void LedNeoPixel::debugColors() {
    LedColor colors[4] = {red, green, blue, no_color};
    for (size_t i = 0; i < 4; i++)
    {
        setColor(colors[i]);
        strip.Show();
        delay(400);
    }
}

void BlendAnimUpdate(const AnimationParam &param)
{
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
}

void LedNeoPixel::FadeInFadeOutRinseRepeat(float luminance, RgbColor target, uint16_t time)
{
    if (fadeToColor)
    {
        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = target;
        animations.StartAnimation(0, time, BlendAnimUpdate);
    }
    else
    {

        uint16_t time = random(600, 700);
        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = RgbColor(0);
        animations.StartAnimation(0, time, BlendAnimUpdate);
    }
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
    setColorRGB(dimmed_color);
}

void LedNeoPixel::darken(RgbColor color) {
    RgbColor dimmed_color = color.Brighten(dimFactor);
    setColorRGB(dimmed_color);
}

void LedNeoPixel::AnimationStart(RgbColor color, uint16_t time) {
    FadeInFadeOutRinseRepeat(0.25, color, time);
}

void LedNeoPixel::update() {
    if (animations.IsAnimating())
    {
        animations.UpdateAnimations();
        strip.Show();
    }
    switch (this->effect) {
        case LedEffect::dim:
            animations.StopAll();
            brighten(actual_color);
            strip.Show();
            break;
        case LedEffect::bright:
            animations.StopAll();
            darken(actual_color);
            strip.Show();
            break;
        case LedEffect::fast_pulse:
            actual_time = fast;
            if (!animations.IsAnimating()) {
                AnimationStart(actual_color, actual_time);
            }
            break;
        case LedEffect::slow_pulse:
            actual_time = slow;
            if (!animations.IsAnimating()) {
                AnimationStart(actual_color, actual_time);
            }
            break;
    }
}