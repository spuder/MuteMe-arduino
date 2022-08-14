#ifndef Led_h
#define Led_h


enum LedEffect {
    bright, // 0
    dim, // 1
    fast_pulse, // 2
    slow_pulse, // 3
};

enum LedColor {
    no_color, // 0
    red, // 1
    green, // 2
    yellow, // 3
    blue, // 4
    purple, // 5
    cyan, // 6
    white, // 7
};

class Led {
    private:
        LedColor color;
        LedEffect effect;
        byte brightness;
        int red_pin, green_pin, blue_pin;
        unsigned long oldMillis;
        unsigned long last_refresh_time;

        void pulse(int period);
        void blink(int period);
        void bright();

        void invertAnalogWrite(int pin, int value);
        byte mapRed(LedColor color);
        byte mapGreen(LedColor color);
        byte mapBlue(LedColor color);
    public:
        Led(byte red_pin, byte green_pin, byte blue_pin);
        void update();
        void setEffect(LedEffect effect);
        void setColor(LedColor color);
};

#endif