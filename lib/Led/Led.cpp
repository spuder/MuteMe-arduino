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
            this->brightness = 255;
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
    unsigned char* colors = convertColor(this->color);
    if (millis() - last_refresh_time > period ){
        this->brightness = 255;

        float percent = (float)this->brightness / 255;
        // Serial.print(percent);
        // Serial.print("% ");
        // Serial.print(colors[0] * percent);
        // Serial.print(",");
        // Serial.print(colors[1] * percent);
        // Serial.print(",");
        // Serial.print(colors[2] * percent);
        // Serial.println("");
        invertAnalogWrite(this->red_pin, colors[0] * percent);
        invertAnalogWrite(this->green_pin, colors[1] * percent);
        invertAnalogWrite(this->blue_pin, colors[2] * percent);
    }
    if (millis() - last_refresh_time > period*2) {
        this->brightness = 70;
        float percent = (float)this->brightness / 255;
        // Serial.print(percent);
        // Serial.print("% ");
        // Serial.print(colors[0] * percent);
        // Serial.print(",");
        // Serial.print(colors[1] * percent);
        // Serial.print(",");
        // Serial.print(colors[2] * percent);
        // Serial.println("");
        invertAnalogWrite(this->red_pin, colors[0] * percent);
        invertAnalogWrite(this->green_pin, colors[1] * percent);
        invertAnalogWrite(this->blue_pin, colors[2] * percent);
        last_refresh_time += 1000;
    }
}

void Led::invertAnalogWrite(int pin, int value) {
    analogWrite(pin, 255 - value);
}

void Led::bright() {
    // Serial.println("running bright");
    // Serial.print("brightness is: ");
    // Serial.println(this->brightness);
    // Serial.println("red pin is: ");
    // Serial.println(this->red_pin);
    invertAnalogWrite(this->red_pin, this->brightness);
}

// function named convertColor to takes 1 parameter of type LedColor and returns a byte arra of length 3
unsigned char* Led::convertColor(LedColor color) {
    unsigned char* color_array = new unsigned char[3];
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
    return color_array;
}