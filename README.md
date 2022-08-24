# MuteMe-Arduino

MuteMe-Arduino is an open source imlementation of the wildly populare [MuteMe hardware device](https://muteme.com/). 

Visually see if you are muted in a zoom/teams meeting, and control your microphone with the touch of a button. 

![](https://imgur.com/ZmdYOMP.png)

![](https://imgur.com/1W8cwSW.png)

![](https://imgur.com/3XjUq6J.png)


## CAD

Gerber files are available in the [Gerber](./Gerber/) directory, and available on [https://oshwlab.com/owenspencer/muteme-arduino](https://oshwlab.com/owenspencer/muteme-arduino)

PCB files are available at [https://oshwlab.com/owenspencer/muteme-arduino](https://oshwlab.com/owenspencer/muteme-arduino)
## Building

**Q: Should you build this?**

Most likely not. An official MuteMe-mini is only $19 with free shipping which is likely about the same or *more* than it would cost to bulid with an arduino. You are highly encouraged to support the origional creators. 


Where this project *does* make sense, is if you have a  mechanical keyboard or macro pad with a micro controller that supports HID emulation, and you want an embedded zoom button. 


- [Adafruit MacroPad](https://www.adafruit.com/product/5128)  
- [Adafruit FeatherWing](https://www.adafruit.com/product/4979)
- [Adafruit NeoPixel](https://www.adafruit.com/product/5020)
- [BYO Keybaord](https://painlessprototyping.com/byo-keyboard/)
- [BYO Keyboard 9 key](https://www.etsy.com/listing/1024993040/byo-9-key-keyboard-kit)
  

## Hardware

This project is designed around an Arduino Pro Mini, though any Arduino with a 32u4 chip should work (you may need to change pin assignments in the platformio.ini build flags. ItsyBitsy/Feather/Teensy should also work. 

- [Arduino Pro Mini](https://docs.arduino.cc/retired/boards/arduino-pro-mini)  
- [Arduino Pro Micro](https://store-usa.arduino.cc/products/arduino-micro)  
- [ItsyBitsy M0](https://www.adafruit.com/product/3727)  
- [Teensy](https://www.pjrc.com/teensy/)  

If you get a board working and want to add it to the `platformio.ini`, pull requests are welcomed and celebrated. 

### Arduino Pro Micro

On an arduino pro micro, connect the buttons and leds to the following pins. 
Put a ~300-400 ohm resistor on each leg of the RGB Led

You can use a common cathode or common anode LED. If using a common cathod you will need to change the variable `rgbLed_TYPE` 

```
#define BUTTON_PIN 2
#define RED_PIN	  6
#define BLUE_PIN  10
#define GREEN_PIN 9
#define rgbLed_TYPE RGBLed::COMMON_ANODE
```

Note: Pull up resistor on button is not required since Arduino Pro mini has internal pull up on pin 2

![](https://imgur.com/g8jYZV3.png)

(Fritzing file available in the [fritzing](./fritzing/) directory)

## Tech Specs

It is possible to have up to 3 MuteMe devices connected to 1 computer simultaniously, as long as they each have a different VID/PID. The VID/PID MuteMe provides are shown below. 

This repo contains platform.io profiles for all 3. (More than likely you should use 'prototype'.) See [platformio.ini](./platformio.ini)

| Name | VID | PID|
|--- | --- | --- |
| Prototype | 0x16c0 | 0x27db |
| Origional | 0x20a0 | 0x42da | 
| Mini | 0x20a0 | 0x42db | 


Note that regardless of what model you choose, you will most likely see this warning. The official MuteMe devices have an undocumented method for differenciating between current models and the origional kickstarter models. If you figure out how to eliminate this warning, please open an issue or pull request. (Most likely the MuteMe software sends a challenge, or it could be detecting usagePage/usage/serial_number in the HID discriptor.)

![](https://imgur.com/jQLlJLS.png)

Additional information available on the [MuteMe](https://muteme.com/pages/muteme-hid-key) website


Official MuteMe VendorID/ProductId/UagePage/Usage
```
hidapitester --list-detail

20A0/42DB: muteme.com - MuteMe-Mini
  vendorId:      0x20A0
  productId:     0x42DB
  usagePage:     0x0009
  usage:         0x0006
  serial_number:  
  interface:     0 
  path: IOService:/AppleARMPE/arm-io/AppleT600xIO/usb-drd1@2280000/AppleT6000USBXHCI@01000000/usb-drd1-port-hs@01100000/MuteMe-Mini@01100000/IOUSBHostInterface@0/AppleUserUSBHostHIDDevice
```

Compare to an arudino pro mini in USB_SERIAL_HID mode (2 virtual devices)

```
16C0/27DB: SparkFun - SparkFun Pro Micro
  vendorId:      0x16C0
  productId:     0x27DB
  usagePage:     0x0001
  usage:         0x0006
  serial_number: CHIDLD 
  interface:     3 
  path: IOService:/AppleARMPE/arm-io/AppleT600xIO/usb-drd2@2280000/AppleT6000USBXHCI@02000000/usb-drd2-port-hs@02100000/USB2.0 Hub@02100000/AppleUSB20Hub@02100000/AppleUSB20HubPort@02110000/SparkFun Pro Micro@02110000/IOUSBHostInterface@3/AppleUserUSBHostHIDDevice

16C0/27DB: SparkFun - SparkFun Pro Micro
  vendorId:      0x16C0
  productId:     0x27DB
  usagePage:     0xFFC0
  usage:         0x0C00
  serial_number: CHIDLD 
  interface:     2 
  path: IOService:/AppleARMPE/arm-io/AppleT600xIO/usb-drd2@2280000/AppleT6000USBXHCI@02000000/usb-drd2-port-hs@02100000/USB2.0 Hub@02100000/AppleUSB20Hub@02100000/AppleUSB20HubPort@02110000/SparkFun Pro Micro@02110000/IOUSBHostInterface@2/AppleUserUSBHostHIDDevice
  ```

### Input

The MuteMe software sends single hex bytes to the device which lights up a corresponding LED. 

```
red = 0x01
green = 0x02
yellow = 0x03
blue = 0x04
purple(magenta) = 0x05
cyan = 0x06
white = 0x07
noColor = 0x00
```

Note, `noColor` on the official devices actually requires 2 bytes `0x00 0x00`

Data is read using the awesome [HID-Project library](https://github.com/NicoHood/HID). 

```
    while (bytesAvailable--) {
      auto data = RawHID.read();
```

You can view the data that MuteMe software sends with the fantastic [hidapitester](https://github.com/todbot/hidapitester) app

Examples: 

Turn LED Blue
```
# Change '20A0/42DB' to your appropriate VID/PID
hidapitester --list-detail
hidapitester --vidpid 20A0/42DB --open --length 1 --send-feature 0x04 --close
```

Turn LED Magenta with fast blink
```

hidapitester --vidpid 20A0/42DB --open --length 1 --send-feature 0x25 --close
```

Turn off LED
```
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0,0 --close
```

### Output

The physical device sends 4 bytes back to the Mute Me software. 

**Note: MuteMe and hidapitester will have conflicts. You must close the MuteMe software to view output from the physical device** 


Note that while we only need to send 4 bytes, [there is a bug in the HID-Project library](https://github.com/NicoHood/HID/issues/133) that requires `64` bytes be sent. 

02 = Button was released event
```
hidapitester --vidpid 20A0/42DB --open --length 4 --read-input

Opening device, vid/pid: 0x20A0/0x42DB
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 02
Closing device
```


To watch all events as they stream by
```
hidapitester --vidpid 20A0:42DB --open --length 64 --read-input-forever --close
```

When 'holding' the button, MuteMe devices send `0x00 0x00 0x00 0x01` every few hundred milliseconds. 
To prevent overwhelming the USB bus with unneccessary data, MuteMe-Arduino sends that byte every 100 milliseonds which seems to be frequent enough. 

```
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 ```

## Debugging

Note: The MuteMe software can only _receive_ input from the arduino if there is only 1 UID per device (e.g. `build_flags = -D USB_HID`). 
Attempting to use `build_flags = -D USB_SERIAL_HID` will allow you to add debugging statements like `Serial.println()`, however it may result in multiple HID devices being registered on a single USB port which can confuse Mute Me software. Any button presses may not be detected by the Mute Me Software while in Serial mode. If you know a way to work arround this limitation, pull requests are welcome and encouraged. 


## Warning

This project is provided for eduacational purposes only. Comercial products are prohibited.  

MuteMe-Arduino is licensed under the [Creative Commons Non Commercial ShareAlike](https://creativecommons.org/licenses/by-nc-sa/4.0/) license. 

