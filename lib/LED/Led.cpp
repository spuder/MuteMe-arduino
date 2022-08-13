#include "Arduino.h"

#include "LED.h"

int Led::RED[3] = {255, 0, 0};
int Led::GREEN[3] = {0, 255, 0};
int Led::BLUE[3] = {0, 0, 255};
int Led::MAGENTA[3] = {255, 0, 255};
int Led::CYAN[3] = {0, 255, 255};
int Led::YELLOW[3] = {255, 255, 0};
int Led::WHITE[3] = {255, 255, 255};

Led::Led(byte pin_red, byte pin_green, byte pin_blue, LedType led_type) {
        m_ledType = led_type; // 0 = common_anode, 1 = common_cathode
        m_redPin = pin_red;
        m_greenPin = pin_green;
        m_bluePin = pin_blue;
        m_brightness = 255;
}
void Led::on(int color[3]) {
     if (m_ledType == 0) {
        analogWrite(m_redPin, 255 - color[0]);
        analogWrite(m_greenPin, 255 - color[1]);
        analogWrite(m_bluePin, 255 - color[2]);
    }
    else {
        analogWrite(m_redPin, color[0]);
        analogWrite(m_greenPin, color[1]);
        analogWrite(m_bluePin, color[2]);
    }
}
void Led::on(int red, int green, int blue) {
    if (m_ledType == 0) {
        analogWrite(m_redPin, 255 - red);
        analogWrite(m_greenPin, 255 - green);
        analogWrite(m_bluePin, 255 - blue);
    }
    else {
        analogWrite(m_redPin, red);
        analogWrite(m_greenPin, green);
        analogWrite(m_bluePin, blue);
    }
}
void Led::off() {
    // 0 = common_anode
    if (m_ledType == 0) {
        analogWrite(m_redPin, 255-0);
        analogWrite(m_greenPin, 255-0);
        analogWrite(m_bluePin, 255-0);
    } else {
        analogWrite(m_redPin, 0);
        analogWrite(m_greenPin, 0);
        analogWrite(m_bluePin, 0);
    }
}