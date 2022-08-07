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