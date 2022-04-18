@mainpage

UDS Bootloader
==============

what it is
----------
This project takes the newly released CHARON UDS Stack by Sentinel Software GmbH and wraps it in a bootloader. Together with Shen Li's ISO TP stack, it is capable of communicating via CAN bus.

At the moment, all drivers and hardware interfaces are designed to be used with a STM32L471xx microcontroller, but it can be ported with low effort to any other STM32L4xx Microcontroller, with some minor changes to drivers even to all STM32 microcontrollers. For use with different cortex-m MCUs the drivers might need to be rewritten. For use with different architecture, an update of linker file and startup code will be necessary.

How to use it / configure it
----------------------------
Compile it and flash it to the start address of your MCU. Application must be linked to 0x08001800, but this can be configured in drvFlash partition lookup table.

Jump into bootloader by writing 0xB00110AD to RAM address 0x10000000. This address can be changed in linker file.

But before you compile:
- Configure the CAN bus pins you want to use in drvCan.c
- Configure the baud rate, UDS receive and transmit ID in isotp_socket.c
- Configure the flash layout in drvFlash.c

Compile it with ceedling as your build toolchain.

Optional Features
-----------------
- Flash is partitioned, so erasing flash will not erase any NVM parameter storage and log files.
- Add your own GPIOs (see sample code in drvGpio.c) (remove sample code if not needed)
- Add a crc algorithm to check validity of your application.

Limitations
-----------
The author is not responsible for any bugs or missing features in the external software library. 

For bugfixes and feature requests regarding CHARON UDS Stack, please file issues in the related github repository (https://github.com/Sentinel-Software-GmbH/CHARON-Server)

If there are new features in the UDS stack, which you would like to see implemented in this bootloader, please file an issue and/or contact me directly.

About this project
------------------
This project was a weekend coding project in the context of a twitch live stream. Follow me on Twitch and contact me there for any questions around this code and project.

https://twitch.tv/LittleBasdart


License
-------
This software is releases under GPLv3. You can get a copy of the license under https://www.gnu.org/licenses/gpl-3.0.html.

You can buy this code and use it for your commercial products without any limitations. Contact me for further information.

Please keep in mind, that this project relies on
- CHARON UDS stack (https://github.com/Sentinel-Software-GmbH/CHARON-Server)
- ISO TP (https://github.com/lishen2/isotp-c)

Consider their licenses, too.

Copyright
---------
This project was developed by Little Basdart (https://twitch.tv/LittleBasdart)
