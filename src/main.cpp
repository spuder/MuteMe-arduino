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
    // Led.update();
    // Led.setEffect(1);
    // Led.setColor(1);
    // Led.setEffect(LedEffect::bright);
    // if (millis() - timer > 1000) {
    //     Serial.println("resetting timer");
    //     timer = millis();
    //     Led.setEffect(LedEffect::dim);
    //     Led.setColor(static_cast<LedColor>(random(0, 3)));
    //     delay(600);
    // }
    // Led.setEffect(LedEffect::dim);
    // Led.update();
    // delay(1000);
    // Led.setEffect(LedEffect::bright);
    // Led.update();
    // delay(1000);

    // analogWrite(6, 127);
    // delay(1000);
    // analogWrite(6, 0);
    // delay(1000);
    // Led.setColor(LedColor::green);
    delay(10);
    Led.update();
}


