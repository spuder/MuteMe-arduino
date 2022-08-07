
// Include Libraries
#include "Arduino.h"
#include "Button.h"
#include "RGBLed.h"


// Pin Definitions
#define PUSHBUTTON_PIN_2	2
#define RGBLED_PIN_B	3
#define RGBLED_PIN_G	5
#define RGBLED_PIN_R	6



// Global variables and defines
#define rgbLed_TYPE COMMON_ANODE
// object initialization
Button pushButton(PUSHBUTTON_PIN_2);
RGBLed rgbLed(RGBLED_PIN_R,RGBLED_PIN_G,RGBLED_PIN_B,rgbLed_TYPE);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    pushButton.init();
    rgbLed.turnOff();              // Start with  LED Strip RGB turned off
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // Mini Pushbutton Switch - Test Code
    //Read pushbutton state. 
    //if button is pressed function will return HIGH (1). if not function will return LOW (0). 
    //for debounce funtionality try also pushButton.onPress(), .onRelease() and .onChange().
    //if debounce is not working properly try changing 'debounceDelay' variable in Button.h
    bool pushButtonVal = pushButton.read();
    Serial.print(F("Val: ")); Serial.println(pushButtonVal);

    }
    else if(menuOption == '2') {
    // RGB Led Common Anode - Test Code
    // The RGB LED will turn PURPLE for 500ms(half a second) and turn off
    rgbLed.setRGB(160, 3, 255);    // 1. sets RGB LED color to purple. Change the values in the brackets to (255,0,0) for pure RED, (0,255,0) for pure GREEN and (0,0,255) for pure BLUE.
    delay(500);                         // 2. change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    rgbLed.setRGB(0, 0, 0);        // 3. turns RGB LED off (showing no color). Change the values in the brackets to alter the color.
    delay(500);                         // 4. change the value in the brackets (500) for a longer or shorter delay in milliseconds.  
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) Mini Pushbutton Switch"));
    Serial.println(F("(2) RGB Led Common Anode"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing Mini Pushbutton Switch"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing RGB Led Common Anode"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/
