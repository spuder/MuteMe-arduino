# include <Arduino.h>
# include <Led.h>

Led::Led(byte red_pin, byte green_pin, byte blue_pin) {
    this->red_pin = red_pin;
    this->green_pin = green_pin;
    this->blue_pin = blue_pin;
    this->effect = LedEffect::bright;
    this->color = LedColor::no_color;
    this->brightness = 0; 
    this->last_refresh_time = 0;
}

/* 
    Led::update() is the state machine that manages the LED state
    These states are sent from the MuteMe software
*/
void Led::update() {
    switch (this->effect) {
        case LedEffect::bright:
            this->brightness = 100;
            shine();
            break;
        case LedEffect::dim:
            this->brightness = 15;
            shine();
            break;
        case LedEffect::fast_pulse:
            pulse(600); // 600 milliseconds = 1 second for 1/2 phase transition
            break;
        case LedEffect::slow_pulse:
            pulse(1200); // 1200 milliseconds = 1 second for 1/2 phase transition
            break;
    }
}

/*
    Led::setEffect is a public function to set the led lighting effect
    https://muteme.com/pages/muteme-hid-key
    0x10 - 0x1F = constant dim
    0x20 - 0x2F = fast pulse
    0x30 - 0x3F = slow pulse
*/
void Led::setEffect(LedEffect effect) {
    this->effect = effect;
}

/*
    Led::setColor is a public function to set the led lighting color
    https://muteme.com/pages/muteme-hid-key
    0x01 = red
    0x02 = green
    0x03 = yellow
    0x04 = blue
    0x05 = purple
    0x06 = cyan
    0x07 = white
    0x00 0x00 = no color
*/
void Led::setColor(LedColor color) {
    this->color = color;
}

/*
    Led::pulse is the fading or breathing animation
    It uses a gausian function to fade the LED from 0 to 255
    Facinating article about the math here: 
    https://makersportal.com/blog/2020/3/27/simple-breathing-led-in-arduino
    https://thingpulse.com/breathing-leds-cracking-the-algorithm-behind-our-breathing-pattern/
*/
void Led::pulse(unsigned int period) {
    if((last_refresh_time + 1) < millis() ){
        last_refresh_time = millis();
        float e = 2.7182818284590452353602874713526624977572470936999595749669676277;
        // min_brightness + max_brightness should equal 255
        byte min_brightness = 35;           // 0 = all the way off, 255 = all the way on
        byte max_brightness = 220;          // 0 = all the way off, 255 = all the way on
        float amplitude = max_brightness / (e - 1/e); //255(e - 1/e); would be 108.0
        float b = (exp(sin(millis()/(float)period * PI )) - 1/(e+ min_brightness)) * amplitude;
        this->brightness = (byte)b;

        byte red_brightness = (mapRed(this->color) * this->brightness) / 255;
        byte green_brightness = (mapGreen(this->color) * this->brightness) / 255;
        byte blue_brightness = (mapBlue(this->color) * this->brightness) / 255;

        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
    }
}

/*
    Led::blink is not used it is simply left here as an option for debugging purposes
    It transitions an LED between 100% and 0% brightness every x number of milliseconds (period)
*/
void Led::blink(unsigned int period) {
    if (millis() - last_refresh_time > period ){
        this->brightness = 100;
        byte red_brightness = (mapRed(this->color) * this->brightness) / 255;
        byte green_brightness = (mapGreen(this->color) * this->brightness) / 255;
        byte blue_brightness = (mapBlue(this->color) * this->brightness) / 255;
        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
    }
    if (millis() - last_refresh_time > period*2) {
        this->brightness = 25;
        byte red_brightness = mapRed(this->color) * this->brightness / 255;
        byte green_brightness = mapGreen(this->color) * this->brightness / 255;
        byte blue_brightness = mapBlue(this->color) * this->brightness / 255;
        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
        last_refresh_time = millis();
    }
}

/*
    Common Cathode LEDS turn 'on' when grounded (e.g. 0v or LOW)
    However the traditional RGB values are 0-255 where 255 = 100% brightness
    Compensate by inverting the RGB values from 0-255 to 255-0
*/
void Led::invertAnalogWrite(int pin, int value) {
    analogWrite(pin, 255 - value);
}

/*
    Led::shine is a normal 'on' or 'dimmed' state
*/
void Led::shine() {
    
    #ifdef DEBUG
    // Print out the color value once a second
    if (millis() - last_refresh_time > 1000) {
        Serial.println("Function is Led::shine()");
        Serial.print("    this->brightness = ");
        Serial.print(this->brightness);
        Serial.print("    rgb *  = ");
        Serial.print(mapRed(this->color) * this->brightness / 100 );
        Serial.print(", ");
        Serial.print(mapGreen(this->color) * this->brightness / 100 );
        Serial.print(", ");
        Serial.print(mapBlue(this->color) * this->brightness / 100);
        Serial.println("");
        last_refresh_time = millis();
    }
    #endif
    invertAnalogWrite(this->red_pin, mapRed(this->color) * this->brightness / 100 );
    invertAnalogWrite(this->green_pin, mapGreen(this->color) * this->brightness / 100);
    invertAnalogWrite(this->blue_pin, mapBlue(this->color) * this->brightness / 100);
}

/*
    Led::mapRed returns the red value of the RGB value of the LED
    [ 255, 0, 0 ] = red
    [ 0, 255, 0 ] = green
    [ 0, 0, 255 ] = blue
    ect...
*/
byte Led::mapRed(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 255;
        case LedColor::green:
            return 0;
        case LedColor::blue:
            return 0;
        case LedColor::yellow:
            return 255;
        case LedColor::purple:
            return 255;
        case LedColor::cyan:
            return 0;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}

/*
    Led::mapGreen returns the green value of the RGB value of the LED
    [ 255, 0, 0 ] = red
    [ 0, 255, 0 ] = green
    [ 0, 0, 255 ] = blue
    ect...
*/
byte Led::mapGreen(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 0;
        case LedColor::green:
            return 255;
        case LedColor::blue:
            return 0;
        case LedColor::yellow:
            return 255;
        case LedColor::purple:
            return 0;
        case LedColor::cyan:
            return 255;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}

/*
    Led::mapBlue returns the blue value of the RGB value of the LED
    [ 255, 0, 0 ] = red
    [ 0, 255, 0 ] = green
    [ 0, 0, 255 ] = blue
    ect...
*/
byte Led::mapBlue(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 0;
        case LedColor::green:
            return 0;
        case LedColor::blue:
            return 255;
        case LedColor::yellow:
            return 0;
        case LedColor::purple:
            return 255;
        case LedColor::cyan:
            return 255;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}
