# include <Arduino.h>
# include <Led.h>

Led::Led(byte red_pin, byte green_pin, byte blue_pin) {
    this->red_pin = red_pin;
    this->green_pin = green_pin;
    this->blue_pin = blue_pin;
    this->effect = LedEffect::bright;
    this->color = LedColor::no_color;
    this->oldMillis = millis();
    this->brightness = 0; 
    this->last_refresh_time = 0;
}

void Led::update() {
    switch (this->effect) {
        case LedEffect::bright:
            this->brightness = 100;
            bright();
            break;
        case LedEffect::dim:
            this->brightness = 50;
            bright();
            break;
        case LedEffect::fast_pulse:
            pulse(600);
            break;
        case LedEffect::slow_pulse:
            break;
    }
}

void Led::setEffect(LedEffect effect) {
    this->effect = effect;
}

void Led::setColor(LedColor color) {
    this->color = color;
}

void Led::pulse(int period) {
    byte* colors = convertColor(this->color);
    if (millis() - last_refresh_time > period ){
        this->brightness = 100;
        invertAnalogWrite(this->red_pin, colors[0] * this->brightness / 100);
        invertAnalogWrite(this->green_pin, colors[1] * this->brightness / 100);
        invertAnalogWrite(this->blue_pin, colors[2] * this->brightness / 100);
    }
    if (millis() - last_refresh_time > period*2) {
        this->brightness = 50;
        Serial.print(" colors[0] is: ");
        Serial.print((int)colors[0]);
        Serial.print(" colors[1] is: ");
        Serial.print((int)colors[1]);
        Serial.print(" colors[2] is: ");
        Serial.print((int)colors[2]);
        Serial.println();

        byte red_brightness = colors[0] * this->brightness / 100;
        byte green_brightness = colors[1] * this->brightness / 100;
        byte blue_brightness = colors[2] * this->brightness / 100;

        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);

        Serial.print("red_brightness is: ");
        Serial.print(red_brightness);
        Serial.print(" green_brightness is: ");
        Serial.print(green_brightness);
        Serial.print(" blue_brightness is: ");
        Serial.println(blue_brightness);

        Serial.println();
        last_refresh_time = millis();
    }
}


void Led::invertAnalogWrite(int pin, int value) {
    // Serial.print("writing ");
    // Serial.print("pin:  ");
    // Serial.print(pin);
    // Serial.print(" value:  ");
    // Serial.print(value);
    // Serial.print(" new value:  ");
    // Serial.println(255-value);
    analogWrite(pin, 255 - value);
}

void Led::bright() {
    invertAnalogWrite(this->red_pin, this->brightness);
}


byte* Led::convertColor(LedColor color) {
    // create a byte array of length 3
    byte color_array[3];
    switch (color) {
        case LedColor::no_color:
            color_array[0] = 0;
            color_array[1] = 0;
            color_array[2] = 0;
            break;
        case LedColor::red:
            color_array[0] = 255;
            color_array[1] = 0;
            color_array[2] = 0;
            break;
        case LedColor::green:
            color_array[0] = 0;
            color_array[1] = 255;
            color_array[2] = 0;
            break;
        case LedColor::blue:
            color_array[0] = 0;
            color_array[1] = 0;
            color_array[2] = 255;
            break;
        case LedColor::yellow:
            color_array[0] = 255;
            color_array[1] = 255;
            color_array[2] = 0;
            break;
        case LedColor::purple:
            color_array[0] = 255;
            color_array[1] = 0;
            color_array[2] = 255;
            break;
        case LedColor::cyan:
            color_array[0] = 0;
            color_array[1] = 255;
            color_array[2] = 255;
            break;
        case LedColor::white:
            color_array[0] = 255;
            color_array[1] = 255;
            color_array[2] = 255;
            break;
    }
    // Serial.println("about to return color array");
    // Serial.print("color_array[0] is: ");
    // Serial.print((int)color_array[0]);
    // Serial.print(" color_array[1] is: ");
    // Serial.print((int)color_array[1]);
    // Serial.print(" color_array[2] is: ");
    // Serial.println((int)color_array[2]);
    // Serial.println("------");
    return color_array;
}