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
        int red_pin, green_pin, blue_pin;
        LedColor color;
        LedEffect effect;
        unsigned char brightness;
        unsigned char max_brightness;
        unsigned long oldMillis;
        void pulse(int period);
        void display(LedColor color, LedEffect effect);
        void bright();
        unsigned long last_refresh_time;
        char* convertColor(LedColor color);
        void invertAnalogWrite(int pin, int value);
    public:
        Led(byte red_pin, byte green_pin, byte blue_pin);
        void update();
        void setEffect(LedEffect effect);
        void setColor(LedColor color);
};

#endif