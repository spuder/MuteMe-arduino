/**

 */
#include "Arduino.h"
#include "Button2.h"
#include "Led.h"
#include "HID-Project.h"

#define BUTTON_PIN 2
#define RED_PIN    6
#define GREEN_PIN  9
#define BLUE_PIN   10

Led Led(RED_PIN, GREEN_PIN, BLUE_PIN );
Button2 button;
byte rawhidData[64];

void pressed(Button2& btn) {
    rawhidData[3] = 0x04; // Touch Button
    RawHID.write(rawhidData, sizeof(rawhidData));
    rawhidData[3] = 0x00;
    #ifdef DEBUG
    Serial.println("pressed");
    #endif
}

void released(Button2& btn) {
    rawhidData[3] = 0x02; // Release Button
    RawHID.write(rawhidData, sizeof(rawhidData));
    rawhidData[3] = 0x00;
    #ifdef DEBUG
    Serial.print("released: ");
    Serial.println(btn.wasPressedFor());
    #endif
}

void longClickDetected(Button2& btn) {
    rawhidData[3] = 0x01; // Hold Button
    RawHID.write(rawhidData, sizeof(rawhidData));
    rawhidData[3] = 0x00;
    #ifdef DEBUG
    Serial.println("long click detected\n");
    #endif 
}


void setup()
{
    Led.setColor(LedColor::blue);
    Led.setEffect(LedEffect::slow_pulse);

    button.begin(BUTTON_PIN);
    button.setPressedHandler(pressed);
    button.setLongClickDetectedHandler(longClickDetected);
    button.setReleasedHandler(released);

    // Workaround for bug when sending less than 64 bytes of data
    for (byte i = 0; i < sizeof(rawhidData); i++)
    {
        rawhidData[i] = 0x00;
    }
    RawHID.begin(rawhidData, sizeof(rawhidData));

    #ifdef DEBUG
    Serial.begin(115200);
    #endif
}

void loop()
{
    Led.update();
    button.loop();

}

