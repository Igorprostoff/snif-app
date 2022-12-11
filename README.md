# WiFi traffic sniffer app

## Functionality
The application intercepts WiFi frames and displays information about captured frames, in particular, the source MAC address, the destination MAC address, the 
MAC address of the wireless access point, the signal strength and channel number. The output of information is carried out on the display of the device and also 
via the COM port.
You can listen to one WiFi channel at a time. Channel switching is carried out by applying a logical zero to the GPIO_0 interface.

## Hardware base
The application uses the ESP32 microcontroller as it's hardware base.

## Software Requirements
To build the code and firmware of the microcontroller, you need to install the ESP-IDF software version 4.4 (or higher) as well as the CMake build system.

## Build and install process
After installing the required software, you need to go to the application directory and enter the following command

     idf.py menuconfig
    
In the menu, you need to select the Project Configuration item and specify the display operation parameters in it (the connection interface and the 
GPIO pins used). Setting example:
![config-example](https://i.ibb.co/g6T3tb9/example-config.png)

After setting up the screen, you need to exit the menu, save the changes and enter the following command:


     idf.py -p /dev/tty.usbserial-0001 flash monitor
    
where:
- idf.py - installed utility for working with ESP microcontrollers
- -p /dev/tty.usbserial-0001 - flag indicating the COM port to which the board is connected (in Windows it can be designated as COM1, COM2 ...)
- flash - an instruction to collect the source code and write it to the device's flash memory
- monitor - an instruction to connect to a device to monitor the output

After that, the device is ready for operation, switching channels in the case of the ESP32 is carried out by pressing the Boot button.

The following information will be displayed on the screen (by line number)
1. Source address
2. Destination address
3. Wireless access point address
4. Signal level
5. Channel number (also notification of channel change)

### The authors
- Prostov I.A.
- Amfiteatrova S.S.
- Project https://github.com/ESP-EOS/ESP32-WiFi-Sniffer
