Arduino Hacking Arduino
=======================

Source file for Arduino MQQT client, part of eMan IoT workshop apps stack (see Related).

Features
--------

1. Connects to the local WiFi and MQTT broker
2. Subscribes to all related topics, lights up LEDs according to received messages
3. Supports both switching on/off for each LED separately via [Arduino Hacking - iOS and watchOS ](https://github.com/eManPrague/arduino-hacking-ios) and lighting up a longer color sequence using [Arduino Hacking - Web](https://github.com/eManPrague/arduino-hacking-web)

Communication
---------------------------

Communication with the MQTT broker takes place via following topics:

1. `arduino/:id/:color` – :color is one of `red`, `green`, `blue` or `yellow`
2. `arduino/:id/stream` – expects a sequence composed of `RGBY` characters. Any other character shuts all the LEDs off.
3. `arduino/:id/status` – online state of the device. Sends `0x01` on connect and `0x00` on disconnect (last will message)

Dependencies
------------

* Requires Arduino compatible device (demo runs on Arduino Uno) with installed WiFi shield.
* [PubSubClient](https://github.com/knolleary/pubsubclient) installed in your IDE

Installation
---------------------------

* Download the IDE of your choice: [Arduino IDE](Download the Arduino Software) / [PlatformIO](http://platformio.org/)
* Make sure you have the latest versions of all the included libraries and your WiFi shield's firmware
* Open `led.ino` file.
* Fill in all the required credentials/details in the config section (WiFi SSID & password, MQQT server address, login etc.)
* Upload the file to your Arduino.

**The app expects your `RGBY` LEDs to be connected to PINs `5`, `4`, `3` and `2`, however this is customizable within the code.**

Related
-------

* [Arduino Hacking - Web](https://github.com/eManPrague/arduino-hacking-web)
* [Arduino Hacking - iOS and watchOS ](https://github.com/eManPrague/arduino-hacking-ios)


License
-------

Arduino Hacking Arduino is released under the [MIT License](http://www.opensource.org/licenses/MIT).
