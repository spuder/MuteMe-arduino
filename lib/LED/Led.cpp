#include "Arduino.h"

#include "LED.h"
Led::Led(byte pin_red, byte pin_green, byte pin_blue, LedType led_type) {
        
        m_ledType = led_type;
        m_redPin = pin_red;
        m_greenPin = pin_green;
        m_bluePin = pin_blue;

}
void Led::on() {
}

void Led::off() {

}