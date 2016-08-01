Arduino Hacking Arduino
=======================

Source file for Arduino MQQT client, part of eMan IoT workshop apps stack (see Related).

Features
--------

1. Receives
2. Forwards received Websocket messages (typically from a web browser) to the MQTT broker

Dependencies
------------

* Requires Arduino compatible device (demo runs on Arduino Uno) with installed WiFi shield.
* [PubSubClient](https://github.com/knolleary/pubsubclient) installed in your IDE

Installation
---------------------------

* Download the IDE of your choice: [Arduino IDE](Download the Arduino Software) / [PlatformIO](http://platformio.org/)
* Open `led.ino` file.
* Fill in all the required credentials/details in the config section (WiFi SSID & password, MQQT server address, login etc.)
* Upload the file to your Arduino.


Related
-------

* [Arduino Hacking - Web](https://github.com/eManPrague/arduino-hacking-web)
* [Arduino Hacking - iOS and watchOS ](https://github.com/eManPrague/arduino-hacking-ios)


License
-------

Arduino Hacking Arduino is released under the [MIT License](http://www.opensource.org/licenses/MIT).
