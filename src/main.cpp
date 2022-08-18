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
    // #ifdef DEBUG
    // Serial.println("pressed");
    // #endif
}

void released(Button2& btn) {
    rawhidData[3] = 0x02; // Release Button
    RawHID.write(rawhidData, sizeof(rawhidData));
    rawhidData[3] = 0x00;
    // #ifdef DEBUG
    // Serial.print("released: ");
    // Serial.println(btn.wasPressedFor());
    // #endif
}

void longClickDetected(Button2& btn) {
    rawhidData[3] = 0x01; // Hold Button
    RawHID.write(rawhidData, sizeof(rawhidData));
    rawhidData[3] = 0x00;
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
    long bytesAvailable = RawHID.available();
    if (bytesAvailable > 0)
    {
        while (bytesAvailable--)
        {
            uint16_t hidData = RawHID.read();
            uint16_t ones = ((byte)hidData / 1)  % 16;
            uint16_t tens = ((byte)hidData / 10) % 16;

            if (ones >=0x00 && ones <= 0x07)
            {
                if (tens >= 0x00 && tens <= 0x03)
                {
                    #ifdef DEBUG
                    Serial.print("hidData: ");
                    Serial.print(hidData, DEC);
                    Serial.print(" / ");
                    Serial.println(hidData, HEX);

                    Serial.print("ones: ");
                    Serial.print(ones, DEC);
                    Serial.print(" / ");
                    Serial.println(ones, HEX);

                    Serial.print("tens: ");
                    Serial.print(tens, DEC);
                    Serial.print(" / ");
                    Serial.println(tens, HEX);

                    Serial.println();
                    #endif
                    parseColor(ones);
                    parseEffect(tens);
                }
            }
            // if ( (ones >= 0 && ones <= 7) && (tens >= 0 && tens <= 3) ) {
                // parseColor(ones);
                // parseEffect(0);
            // } else {
            //     #ifdef DEBUG
            //     Serial.print("Invalid/Undocumented data: ");
            //     Serial.print(ones);
            //     Serial.print(tens);
            //     Serial.print("/");
            //     Serial.println(hidData, HEX); //TODO: display as hex/byte
            //     #endif
            // }

        }
    }
}

