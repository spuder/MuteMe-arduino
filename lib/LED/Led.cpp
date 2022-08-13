#include "Arduino.h"

#include "LED.h"
Led::Led(byte pin_red, byte pin_green, byte pin_blue, int foobar) {
        
        m_ledType = foobar;
        m_redPin = pin_red;
        m_greenPin = pin_green;
        m_bluePin = pin_blue;

}
void Led::on() {
}

void Led::off() {

}