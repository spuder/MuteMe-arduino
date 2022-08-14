/**

 */
#include "Arduino.h"
#include "Led.h"

#define RED_PIN    6
#define GREEN_PIN  9
#define BLUE_PIN   10

Led Led(RED_PIN, GREEN_PIN, BLUE_PIN );

unsigned long timer = 0;
void setup()
{
    timer = millis();
    Led.setColor(LedColor::green);
    Led.setEffect(LedEffect::fast_pulse);
    Serial.begin(9600);
}

void loop()
{
    Led.update();
}


