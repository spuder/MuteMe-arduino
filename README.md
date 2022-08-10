# MuteMe-Arduino

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



## Debugging

Note: The MuteMe software can only _receive_ input from the arduino if there is only 1 UID per device (e.g. `build_flags = -D USB_HID`). 
Attempting to use `build_flags = -D USB_SERIAL_HID` will allow you to add debugging statements like `Serial.println()`, however it will result in multiple HID devices being registered on a single USB port which will confuse Mute Me. Any button presses will not be detected by the Mute Me Software. If you know a way to work arround this, pull requests welcome and encouraged. 