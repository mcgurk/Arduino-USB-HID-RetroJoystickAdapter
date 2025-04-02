## Windows USB Descriptor dumper
https://www.thesycon.de/eng/usb_descriptordumper.shtml


## USB descriptor from workin adapter
```
Information for device Arduino Leonardo (VID=0x2341 PID=0x8036):

------------------------------
Connection Information:
------------------------------
Device current bus speed: FullSpeed
Device supports USB 1.1 specification
Device supports USB 2.0 specification
Device address: 0x0039
Current configuration value: 0x01
Number of open pipes: 1


------------------------------
Device Descriptor:
------------------------------
0x12	bLength
0x01	bDescriptorType
0x0200	bcdUSB
0x00	bDeviceClass      
0x00	bDeviceSubClass   
0x00	bDeviceProtocol   
0x40	bMaxPacketSize0   (64 bytes)
0x2341	idVendor
0x8036	idProduct
0x0100	bcdDevice
0x01	iManufacturer   "Arduino LLC"
0x02	iProduct        "Arduino Leonardo"
0x03	iSerialNumber   "CD32/C= to USB"
0x01	bNumConfigurations


-------------------------
Configuration Descriptor:
-------------------------
0x09	bLength
0x02	bDescriptorType
0x0022	wTotalLength   (34 bytes)
0x01	bNumInterfaces
0x01	bConfigurationValue
0x00	iConfiguration
0xA0	bmAttributes   (Bus-powered Device, Remote-Wakeup)
0xFA	bMaxPower      (500 mA)

Interface Descriptor:
------------------------------
0x09	bLength
0x04	bDescriptorType
0x00	bInterfaceNumber
0x00	bAlternateSetting
0x01	bNumEndPoints
0x03	bInterfaceClass      (Human Interface Device Class)
0x00	bInterfaceSubClass   
0x00	bInterfaceProtocol   
0x00	iInterface

HID Descriptor:
------------------------------
0x09	bLength
0x21	bDescriptorType
0x0101	bcdHID
0x00	bCountryCode
0x01	bNumDescriptors
0x22	bDescriptorType   (Report descriptor)
0x002F	bDescriptorLength

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x81	bEndpointAddress  (IN endpoint 1)
0x03	bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
0x0040	wMaxPacketSize    (1 x 64 bytes)
0x01	bInterval         (1 frames)

Microsoft OS Descriptor is not available. Error code: 0x0000001F


--------------------------------
String Descriptor Table
--------------------------------
Index  LANGID  String
0x00   0x0000  0x0409 
0x01   0x0409  "Arduino LLC"
0x02   0x0409  "Arduino Leonardo"
0x03   0x0409  "CD32/C= to USB"

------------------------------

Connection path for device: 
USB xHCI Compliant Host Controller
Root Hub
Arduino Leonardo (VID=0x2341 PID=0x8036) Port: 8

Running on: Windows 10 or greater (Build Version 19045)

Brought to you by TDD v2.19.0, Dec  5 2023, 12:08:38
```

## Not working descriptor
```
Information for device McGurk Retro Joystick Adapter (VID=0x8282 PID=0x3201):

------------------------------
Connection Information:
------------------------------
Device current bus speed: FullSpeed
Device supports USB 1.1 specification
Device supports USB 2.0 specification
Device address: 0x003C
Current configuration value: 0x01
Number of open pipes: 4


------------------------------
Device Descriptor:
------------------------------
0x12	bLength
0x01	bDescriptorType
0x0200	bcdUSB
0xEF	bDeviceClass      (Miscellaneous device)
0x02	bDeviceSubClass   
0x01	bDeviceProtocol   
0x40	bMaxPacketSize0   (64 bytes)
0x8282	idVendor
0x3201	idProduct
0x0100	bcdDevice
0x01	iManufacturer   "Unknown"
0x02	iProduct        "McGurk Retro Joystick Adapter"
0x03	iSerialNumber   "HIDOF"
0x01	bNumConfigurations


-------------------------
Configuration Descriptor:
-------------------------
0x09	bLength
0x02	bDescriptorType
0x0064	wTotalLength   (100 bytes)
0x03	bNumInterfaces
0x01	bConfigurationValue
0x00	iConfiguration
0xA0	bmAttributes   (Bus-powered Device, Remote-Wakeup)
0xFA	bMaxPower      (500 mA)

Interface Association Descriptor:
------------------------------
0x08	bLength
0x0B	bDescriptorType
0x00	bFirstInterface
0x02	bInterfaceCount
0x02	bFunctionClass      (Communication Device Class)
0x02	bFunctionSubClass   (Abstract Control Model - ACM)
0x00	bFunctionProtocol   
0x00	iFunction

Interface Descriptor:
------------------------------
0x09	bLength
0x04	bDescriptorType
0x00	bInterfaceNumber
0x00	bAlternateSetting
0x01	bNumEndPoints
0x02	bInterfaceClass      (Communication Device Class)
0x02	bInterfaceSubClass   (Abstract Control Model - ACM)
0x00	bInterfaceProtocol   
0x00	iInterface

CDC Header Functional Descriptor:
------------------------------
0x05	bFunctionalLength
0x24	bDescriptorType
0x00	bDescriptorSubtype
0x0110	bcdCDC

CDC Call Management Functional Descriptor:
------------------------------
0x05	bFunctionalLength
0x24	bDescriptorType
0x01	bDescriptorSubtype
0x01	bmCapabilities
0x01	bDataInterface

CDC Abstract Control Management Functional Descriptor:
------------------------------
0x04	bFunctionalLength
0x24	bDescriptorType
0x02	bDescriptorSubtype
0x06	bmCapabilities

CDC Union Functional Descriptor:
------------------------------
0x05	bFunctionalLength
0x24	bDescriptorType
0x06	bDescriptorSubtype
0x00	bControlInterface
0x01	bSubordinateInterface(0)

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x81	bEndpointAddress  (IN endpoint 1)
0x03	bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
0x0010	wMaxPacketSize    (1 x 16 bytes)
0x40	bInterval         (64 frames)

Interface Descriptor:
------------------------------
0x09	bLength
0x04	bDescriptorType
0x01	bInterfaceNumber
0x00	bAlternateSetting
0x02	bNumEndPoints
0x0A	bInterfaceClass      (CDC Data)
0x00	bInterfaceSubClass   
0x00	bInterfaceProtocol   
0x00	iInterface

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x02	bEndpointAddress  (OUT endpoint 2)
0x02	bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
0x0040	wMaxPacketSize    (64 bytes)
0x00	bInterval         

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x83	bEndpointAddress  (IN endpoint 3)
0x02	bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
0x0040	wMaxPacketSize    (64 bytes)
0x00	bInterval         

Interface Descriptor:
------------------------------
0x09	bLength
0x04	bDescriptorType
0x02	bInterfaceNumber
0x00	bAlternateSetting
0x01	bNumEndPoints
0x03	bInterfaceClass      (Human Interface Device Class)
0x00	bInterfaceSubClass   
0x00	bInterfaceProtocol   
0x00	iInterface

HID Descriptor:
------------------------------
0x09	bLength
0x21	bDescriptorType
0x0101	bcdHID
0x00	bCountryCode
0x01	bNumDescriptors
0x22	bDescriptorType   (Report descriptor)
0x005E	bDescriptorLength

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x84	bEndpointAddress  (IN endpoint 4)
0x03	bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
0x0040	wMaxPacketSize    (1 x 64 bytes)
0x01	bInterval         (1 frames)

Microsoft OS Descriptor is not available. Error code: 0x0000001F


--------------------------------
String Descriptor Table
--------------------------------
Index  LANGID  String
0x00   0x0000  0x0409 
0x01   0x0409  "Unknown"
0x02   0x0409  "McGurk Retro Joystick Adapter"
0x03   0x0409  "HIDOF"

------------------------------

Connection path for device: 
USB xHCI Compliant Host Controller
Root Hub
McGurk Retro Joystick Adapter (VID=0x8282 PID=0x3201) Port: 8

Running on: Windows 10 or greater (Build Version 19045)

Brought to you by TDD v2.19.0, Dec  5 2023, 12:08:38
```
