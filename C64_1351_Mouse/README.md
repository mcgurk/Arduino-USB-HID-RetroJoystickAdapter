9 and 10 needed for timer. Move SS and INT of USB host shield away:
```
C:\Users\xxxxx\Documents\Arduino\libraries\USB_Host_Shield_Library_2.0\UsbCore.h:
//Arduino Pro Micro SS -> A0, INT -> A1:
typedef MAX3421e<P18, P19> MAX3421E;
```
