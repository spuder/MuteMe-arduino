#ifndef Led_h
#define Led_h

// #include "Arduino.h"



enum class LedType {
    common_anode,
    common_cathode
};

class Led {
    private:
        int m_redPin, m_greenPin, m_bluePin;
        float m_brightness;
        int m_ledType;

    public:
        Led(byte pin_red, byte pin_green, byte pin_blue, int foobar);
        void on();
        void off();
        void breath(byte speed);
};

#endif
