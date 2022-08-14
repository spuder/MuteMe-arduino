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
            this->brightness = this->max_brightness;
            bright();
            break;
        case LedEffect::dim:
            this->brightness = 170;
            bright();
            break;
        case LedEffect::fast_pulse:
            // this->pulse(this->color, 600);
            pulse(600);
            break;
        case LedEffect::slow_pulse:
            // this->pulse(this->color, 1200 );
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
    // unsigned long currentMillis = millis();
    auto colors = convertColor(this->color);
    if (millis() - last_refresh_time > period ){
        this->brightness = 0;
        auto colors = convertColor(this->color);
        analogWrite(this->red_pin, colors[0] + this->brightness);
        analogWrite(this->green_pin, colors[1] + this->brightness);
        analogWrite(this->blue_pin, colors[2] + this->brightness);

    }
    if (millis() - last_refresh_time > period*2) {
        this->brightness = 255;
        analogWrite(this->red_pin, colors[0] + this->brightness);
        analogWrite(this->green_pin, colors[1] + this->brightness);
        analogWrite(this->blue_pin, colors[2] + this->brightness);
        last_refresh_time += 1000;
    }
}

void Led::bright() {
    Serial.println("running bright");
    Serial.print("brightness is: ");
    Serial.println(this->brightness);
    Serial.println("red pin is: ");
    Serial.println(this->red_pin);
    analogWrite(this->red_pin, this->brightness);
}

// function named convertColor to takes 1 parameter of type LedColor and returns a byte arra of length 3
char* Led::convertColor(LedColor color) {
    byte* color_array = new byte[3];
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