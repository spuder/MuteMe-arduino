#ifndef LedNeoPixel_h
#define LedNeoPixel_h
#include "Arduino.h"
#include <Led.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

class LedNeoPixel {
    private:
        LedColor color;
        LedEffect effect;
        void AnimationControl(RgbColor color, uint16_t time);
        void brighten(RgbColor color);
        void darken(RgbColor color);
        // void BlendAnimUpdate(const AnimationParam &param);
        void FadeInFadeOutRinseRepeat(float luminance, RgbColor target, uint16_t time);
        void setColorRgb(RgbColor color);
        RgbColor actual_color;
    public:
        void initStrip();
        void update();
        void setEffect(LedEffect effect);
        void setColor(LedColor color);
};
#endif