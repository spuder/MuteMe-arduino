#ifndef Led_h
#define Led_h

enum LedType {
    common_anode, // 0 
    common_cathode // 1
};

class Led {
    private:
        int m_redPin, m_greenPin, m_bluePin;
        float m_brightness;
        LedType m_ledType;   
        unsigned long m_lastUpdate;     

    public:
        Led(byte pin_red, byte pin_green, byte pin_blue, LedType led_type);
        void on();
        void on(int color[3]);
        void on(int red, int green, int blue);
        void off();
        // void breath(int speed);
        void blink(int onTime, int offTime);
        static int RED[3];
        static int GREEN[3];
        static int BLUE[3];
        static int MAGENTA[3];
        static int CYAN[3];
        static int YELLOW[3];
        static int WHITE[3];
};

#endif
