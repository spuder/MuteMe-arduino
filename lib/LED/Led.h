#ifndef Led_h
#define Led_h

// #include "Arduino.h"



enum LedType {
    common_anode,
    common_cathode
};

class Led {
    private:
        int m_redPin, m_greenPin, m_bluePin;
        float m_brightness;
        LedType m_ledType;

    public:
        Led(byte pin_red, byte pin_green, byte pin_blue, LedType foobar);
        void on();
        void off();
        void breath(byte speed);
};

#endif
