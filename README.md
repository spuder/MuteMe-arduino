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

There are multiple options for debugging USB Connections. All options are terrible

Listed in order of least amount of terribleness

1. hidapitester https://github.com/todbot/hidapitester  
2. rawhid.py https://www.pjrc.com/teensy/rawhid.html  
3. rawhid (c++) https://www.pjrc.com/teensy/rawhid.html  

