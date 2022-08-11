# ZoomMute

Use a microcontroller to mute zoom meetings



## Resources

- [Get PID/VID of usb device](https://kb.synology.com/en-ph/DSM/tutorial/How_do_I_check_the_PID_VID_of_my_USB_device#:~:text=To%20find%20PID%20%26%20VID%20on%20Mac%3A,view%20its%20PID%20and%20VID.)


Arduino nano

```
  Product ID:	0x0487
  Vendor ID:	0x16c0
```

Mute Me

| Name | VID | PID|
|--- | --- | --- |
| Prototype | 0x16c0 | 0x27db |
| Origional | 0x20a0 | 0x42da | 
| Mini | 0x20a0 | 0x42db | 



Find the boards.txt and change settings

```
mdfind boards.txt
/Users/spencer.owen/Library/Arduino15/packages/arduino/hardware/avr/1.8.5/boards.txt
/Users/spencer.owen/Library/Arduino15/packages/esp8266/hardware/esp8266/3.0.2/tools/boards.txt.py
/Users/spencer.owen/Library/Arduino15/packages/esp8266/hardware/esp8266/3.0.2/boards.txt
/Users/spencer.owen/Code/github/spuder/klipper/docs/Bootloaders.md
```

- https://community.platformio.org/t/changing-usb-device-vid-pid/3986/17


List USB Devices (OSX)

`system_profiler SPUSBDataType`

```
USB Serial:

  Product ID: 0x0483
  Vendor ID: 0x16c0
  Version: 2.71
  Serial Number: 12345
  Speed: Up to 12 Mb/s
  Manufacturer: Teensyduino
  Location ID: 0x01110000 / 5
  Current Available (mA): 500
  Current Required (mA): 100
  Extra Operating Current (mA): 0
```

Setting Product ID and Vendor ID

- Use FT Prog (windows only) https://ftdichip.com/utilities/

https://community.platformio.org/t/changing-usb-device-vid-pid/3986/23?u=owenspencer

HID Data

Once every 5 seconds, MuteMe sends the following

- 19
- 83

Unmuting

- 23
- 87

Muting

- 19
- 83


https://www.hackster.io/abratchik/hid-compliant-ups-with-arduino-75198c

The creator of the Arduino HID library doesn't support it and recomends the adafruit LUFA library
https://github.com/NicoHood/HID/issues/139#issuecomment-692211209

Change arduino to show up as a UPS
https://github.com/abratchik/HIDPowerDevice/blob/master/linux/98-upower-hid.rules

Macro Keyboard
http://www.retrobuiltgames.com/the-build-page/macro-keyboard-v2-0/macro-keyboard-v0-4/


## Debugging

There are multiple options for debugging USB Connections. All options are terrible

Listed in order of least amount of terribleness

1. hidapitester https://github.com/todbot/hidapitester  
2. rawhid.py https://www.pjrc.com/teensy/rawhid.html  
3. rawhid (c++) https://www.pjrc.com/teensy/rawhid.html  



```
hidapitester --list
hidapitester --list-detail
hidapitester --vidpid 16C0/27DB --open --send-output 0x00
hidapitester --vidpid 16C0/27DB --usagePage 0001 --usage 0006 --open
hidapitester --vidpid 16C0/27DB --usagePage FFC0 --usage 0C00 --open #Note more than 1 sparkfun device may appear, use usagePage and Usage to more finely refine which one you want

hidapitester --vidpid 16C0/27DB --usagePage FFC0 --usage 0C00 --open --send-feature 01 --close
hidapitester --vidpid 16C0/27DB --usagePage FFC0 --usage 0C00 --open --send-feature 0x01 --close --length 64
hidapitester --vidpid 16C0/27DB --usagePage 0001 

```

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




Example: Controlling LED with mute me mini (20A0/42DB)
```
# List Devices
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


- You can use either `4` or `0x04` syntax
- You can send any length you like (1-255), but turning off requires length >=2 
```
# Turn LED Blue (0x04)
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0x04 --close

# Turn LED Purple with fast blink (0x05 + 0x20)
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0x25 --close

# Turn LED White
hidapitester --vidpid 20A0/42DB --open --length 1 --send-feature 0x07 --close

# Turn off LED (requires length >1). All the following syntaxes work
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0x00 0x00 --close
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0,0 --close
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0,0 --close
hidapitester --vidpid 20A0/42DB --open --length 2 --send-feature 0 --close
```


- Note that the input always ends on the 4th byte? Very curious
```
# Physically touch the MuteMe, then run the following 
# (you only have 1/4 of a second to catch it so its really hard to catch the touch/release events)
# This will capture 16 bytes
hidapitester --vidpid 20A0/42DB --open --length 17 --read-input 3

Opening device, vid/pid: 0x20A0/0x42DB
Reading 17-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00
Closing device

hidapitester --vidpid 20A0/42DB --open --length 64 --read-input
```


```
# Captured a touch event (0x00 0x00 0x00 0x04)
hidapitester --vidpid 20A0/42DB --open --length 4 --read-input

Opening device, vid/pid: 0x20A0/0x42DB
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 04
Closing device
```

```
# Captured a release event (0x00 0x00 0x00 0x02)

hidapitester --vidpid 20A0/42DB --open --length 4 --read-input

Opening device, vid/pid: 0x20A0/0x42DB
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 02
Closing device
```

```
hidapitester --vidpid 20A0/42DB --close
```



16C0/27DB: SparkFun - SparkFun Pro Micro
  vendorId:      0x16C0
  productId:     0x27DB
  usagePage:     0xFFC0
  usage:         0x0C00
  serial_number: CHIDLD 
  interface:     2 
  path: IOService:/AppleARMPE/arm-io/AppleT600xIO/usb-drd2@2280000/AppleT6000USBXHCI@02000000/usb-drd2-port-hs@02100000/USB2.0 Hub@02100000/AppleUSB20Hub@02100000/AppleUSB20HubPort@02110000/SparkFun Pro Micro@02110000/IOUSBHostInterface@2/AppleUserUSBHostHIDDevice
16C0/27DB: SparkFun - SparkFun Pro Micro
  vendorId:      0x16C0
  productId:     0x27DB
  usagePage:     0x0001
  usage:         0x0006
  serial_number: CHIDLD 
  interface:     3 
  path: IOService:/AppleARMPE/arm-io/AppleT600xIO/usb-drd2@2280000/AppleT6000USBXHCI@02000000/usb-drd2-port-hs@02100000/USB2.0 Hub@02100000/AppleUSB20Hub@02100000/AppleUSB20HubPort@02110000/SparkFun Pro Micro@02110000/IOUSBHostInterface@3/AppleUserUSBHostHIDDevice

```
hidapitester --list-detail| grep -A 7 'SparkFun - SparkFun Pro Micro'

hidapitester --vidpid 16C0:27DB --usagePage FFC0 --usage 0C00 --open --length 4 --read-input --close

Opening device, vid/pid:0x16C0/0x27DB, usagePage/usage: FFC0/C00
Device opened
Reading 4-byte input report 0, 250 msec timeout...read 4 bytes:
 00 00 00 04
Closing device
```

```
hidapitester --vidpid 20A0/42DB --open

hidapitester --vidpid 20A0/42DB --usagePage 0009 --usage 0006 --open --length 4 --read-input --close
hidapitester --vidpid 20A0/42DB --open --length 4 --read-input --close

hidapitester --vidpid 20A0:42DB --open --length 4 --read-input --close


```

hidapitester --close


All this only works if the muteme app is closed ! 




hidapitester --vidpid 16C0:27DB --usagePage FFC0 --usage 0C00 --open --length 4 --read-input-forever --close

hidapitester --vidpid 20A0:42DB --open --length 64 --read-input-forever --close


Full touch sequence 04 (start touch), 01 (touching), 02 (end touch)

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
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Reading 64-byte input report 0, 250 msec timeout...read 8 bytes:
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 ```


 Need to hook arduino up to pin that supports external interupts

 https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro

 Arudino Pro Micro has

 - pin 3 = INT 0
 - pin 2 = INT 1
 - pin 0 = INT 2
 - pin 1 = INT 3
 - pin 7 = INT 4

https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro


USB VID:PID=16C0:27DB SER=C LOCATION=1-1.1

hidapitester --vidpid 16C0:27DB --open
hidapitester --vidpid 16C0:27DB --open --length 64 --read-input-forever --close
hidapitester --vidpid 16C0/27DB --usagePage FFC0 --usage 0C00 --open --length 64 --read-input-forever --close
hidapitester --vidpid 16C0:27DB --usagePage FFC0 --usage 0C00 --open --length 64 --read-input-forever --close


hidapitester --vidpid 16C0:27DB --open --length 64 --read-input-forever --close