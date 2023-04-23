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
// These are automatically zero initialized
// This is our output buffer used by button presses
byte rawhidWriteBuffer[64]; 
// This is our input buffer used by RawHID
byte rawhidInBuffer[64];
// This is our 64b buffer to make sure we get 64b of input before processing
byte rawhidBuffer[64];
// Byte count to track input
int byte_count = 0;

void pressed(Button2& btn) {
    rawhidWriteBuffer[3] = 0x04; // Touch Button
    RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
    rawhidWriteBuffer[3] = 0x00;
    // #ifdef DEBUG
    // Serial.println("pressed");
    // #endif
}

void released(Button2& btn) {
    rawhidWriteBuffer[3] = 0x02; // Release Button
    RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
    rawhidWriteBuffer[3] = 0x00;
    // #ifdef DEBUG
    // Serial.print("released: ");
    // Serial.println(btn.wasPressedFor());
    // #endif
}

void longClickDetected(Button2& btn) {
    rawhidWriteBuffer[3] = 0x01; // Hold Button
    RawHID.write(rawhidWriteBuffer, sizeof(rawhidWriteBuffer));
    rawhidWriteBuffer[3] = 0x00;
    #ifdef DEBUG
    Serial.println("long click detected\n");
    #endif 
}

void parseColor(int data) {
    switch (data) {
        case 0:
            Led.setColor(LedColor::no_color);
            break;
        case 1:
            Led.setColor(LedColor::red);
            break;
        case 2:
            Led.setColor(LedColor::green);
            break;
        case 3:
            Led.setColor(LedColor::yellow);
            break;
        case 4:
            Led.setColor(LedColor::blue);
            break;
        case 5:
            Led.setColor(LedColor::purple);
            break;
        case 6:
            Led.setColor(LedColor::cyan);
            break;
        case 7:
            Led.setColor(LedColor::white);
            break;
        // Don't set a default case, since there appear to be some undocumented values
        // sent fromt the software. We want to ignore those
    }
}
void parseEffect(int data) {
    #ifdef DEBUG
    Serial.print("parseEffect: ");
    Serial.println(data);
    #endif
    switch (data) {
        case 0:
            Serial.println("bright");
            Led.setEffect(LedEffect::bright);
            break;
        case 1:
            Serial.println("dim");
            Led.setEffect(LedEffect::dim);
            break;
        case 2:
            Serial.println("fast_pulse");
            Led.setEffect(LedEffect::fast_pulse);
            break;
        case 3:
            Serial.println("slow_pulse");
            Led.setEffect(LedEffect::slow_pulse);
            break;
        // Don't set a default case, since there appear to be some undocumented values
        // sent fromt the software. We want to ignore those

    }
}


void setup()
{
    Led.setColor(LedColor::no_color);
    Led.setEffect(LedEffect::bright);

    button.begin(BUTTON_PIN);
    button.setPressedHandler(pressed);
    button.setLongClickDetectedHandler(longClickDetected);
    button.setReleasedHandler(released);

    // Workaround for bug when sending less than 64 bytes of data
    // Since arrays are zero init'd by default we shouldn't have to do this anymore
    /* for (byte i = 0; i < sizeof(rawhidWriteBuffer); i++)
    {
        rawhidWriteBuffer[i] = 0x00;
    } */

    // There was a potential conflict before if you hit the button fast you could read in
    // Garbage data from HID because the buffer would get overwritten by output
    RawHID.begin(rawhidInBuffer, sizeof(rawhidInBuffer));

    #ifdef DEBUG
    Serial.begin(115200);
    #endif
}

void loop()
{
    Led.update();
    button.loop();
    int bytesAvailable = RawHID.available();
    if (bytesAvailable > 0)
    {
        while (bytesAvailable--)
        {
            if(byte_count < 64) {
                rawhidBuffer[byte_count] = RawHID.read();
                byte_count++;
            } else {
                byte_count = 0;
                // We only care about the first byte.
                int hidData = rawhidBuffer[0];
                int ones = ((byte)hidData / 1)  % 16;
                int tens = ((byte)hidData / 16) % 16;

                if ((ones >=0 && ones <= 7) && (tens >= 0 && tens <= 3) ) 
                {
                    #ifdef DEBUG
                    Serial.print("hidData: ");
                    Serial.print(hidData, DEC);
                    Serial.print(" / 0x");
                    Serial.println(hidData, HEX);

                    Serial.print("ones: ");
                    Serial.print(ones, DEC);
                    Serial.print(" / 0x");
                    Serial.println(ones, HEX);

                    Serial.print("tens: ");
                    Serial.print(tens, DEC);
                    Serial.print(" / 0x");
                    Serial.println(tens, HEX);

                    Serial.println();
                    #endif
                    parseColor(ones);
                    parseEffect(tens);
                }  else {
                    #ifdef DEBUG
                    Serial.print("Invalid/Undocumented data: ");
                    Serial.print(ones);
                    Serial.print(tens);
                    Serial.print("/0x");
                    Serial.println(hidData, HEX);
                    #endif
                }
            }
        }
    }
}

