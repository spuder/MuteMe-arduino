/**

 */
#include "Arduino.h"
#include "Button2.h"
#include "Led.h"

#define BUTTON_PIN 2
#define RED_PIN    6
#define GREEN_PIN  9
#define BLUE_PIN   10

Led Led(RED_PIN, GREEN_PIN, BLUE_PIN );
Button2 button;

unsigned long timer = 0;


void pressed(Button2& btn) {
    #ifdef DEBUG
    Serial.println("pressed");
    #endif
}

void released(Button2& btn) {
    #ifdef DEBUG
    Serial.print("released: ");
    Serial.println(btn.wasPressedFor());
    #endif
}

void longClickDetected(Button2& btn) {
    #ifdef DEBUG
    Serial.println("long click detected\n");
    #endif 
}


void setup()
{
    timer = millis();
    Led.setColor(LedColor::blue);
    Led.setEffect(LedEffect::slow_pulse);

    button.begin(BUTTON_PIN);
    button.setPressedHandler(pressed);
    button.setLongClickDetectedHandler(longClickDetected);
    button.setReleasedHandler(released);

    #ifdef DEBUG
    Serial.begin(115200);
    #endif
}

void loop()
{
    // Led.update();
    button.loop();

}

