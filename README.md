# GPS Receiver ZOE-MQ8 with LCD Display

[![Arduino Compile Sketches](https://github.com/Andy4495/GPS-Breakout-with-LCD/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/GPS-Breakout-with-LCD/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/GPS-Breakout-with-LCD/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/GPS-Breakout-with-LCD/actions/workflows/CheckMarkdownLinks.yml)

GPS Receiver hardware and sketch using Tiva TM4C LaunchPad, SparkFun ZOE-M8Q GPS breakout, and CrystalFonz 8x2 LCD. The sketch is based on Example12_UseUart.ino by Nathan Seidle from the [SparkFun u-blox Arduino Library][6].

## Implementation

The ZOE-M8Q I2C interface is unreliable in my setup. I am not sure if this is a problem with the    hardware or the SparkFun library. This sketch uses UART to communicate between the LaunchPad and the GPS module, without issue.
The Tiva TM4C LaunchPad has 8 available hardware serial ports. This sketch uses UART module 1 (accessed using the "Serial1" object). Debug information is sent through the usual "Serial" USB backchannel.

The LCD display cycles between several GPS data values (lat/long, altitude, # satellites, ground speed, date/time). Hold down "SW1" on the LaunchPad to freeze the display at the current reading.

## Hardware

The GPS can LCD are connected to the LaunchPad as follows:

```text
LaunchPad   GPS  LCD    Signal Name
---------   ---  ---  ----------------
     1       4          3V3
     2       
     3       2          LP RX1 <- GPS TXO
     4       3          LP TX1 -> GPS RXI
     5
     6
     7
     8
     9       7          SCL (This sketch does not use I2C)
    10       8          SDA (This sketch does not use I2C)
    11           4      RS
    12           6      E
    13          11      DB4
    14          12      DB5
    15          13      DB6
    16
    17
    18          14      DB7
    19
    20      6    1,5    GND (LCD pin 5 is R/W*)
    21           2      +5V (VDD)
                 3      Vo (wiper pin on 10K pot tied between +5V and GND)
```

## External Libraries

- SparkFun [u-blox Arduino Library][6]

  This sketch uses SparkFun's [u-blox Arduino library][6]. That library has since been deprecated and replaced with SparkFun's [u-blox GNSS library][7]. Migrating to the new library requires some minor sketch updates as outlined [here][8].

## References

- SparkFun ZOE-M8Q [GPS Breakout][1]
- Texas Instruments [TM4C LaunchPad][2]
  - Embedded Computing [pinout diagram][5]
  - Note that RX1/TX1 on pins 37/36 should be labeled as RX4/TX4
- CrystalFonz [LCD][3]
  - [datasheet][4]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://www.sparkfun.com/products/15193
[2]:https://www.ti.com/tool/EK-TM4C123GXL
[3]:https://www.crystalfontz.com/product/cfah0802anygjt-display-module-text-8x2
[4]:https://www.crystalfontz.com/products/document/891/CFAH0802A-NYG-JTDatasheet.pdf
[5]:https://embeddedcomputing.weebly.com/launchpad-stellaris-lm4f120-pins-maps.html
[6]:https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library
[7]:https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library
[8]:https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library#migrating-to-v20
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/GPS-Breakout-with-LCD
