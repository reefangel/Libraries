Arduino Ethernet
================

Arduino Ethernet is a collection of libraries for use within the Arduino programming environment in conjunction with the Ethernet shield.

This project is an updating of the Arduino Ethernet libraries originally produced by Georg Kaindl. The original versions of the code can be found at http://gkaindl.com/software/arduino-ethernet.

The code has been slightly reorganized from the original libraries. All three libraries (EthernetBonjour, EthernetDHCP, and EthernetDNS) have been combined into a single top level folder, and all examples have been combined as well. The utility classes have be de-duped, too.

Additional changes were made to correct compilation errors when using the libraries with Arduino 1.0.1. This version of the code has only been tested with Arduino 1.0.1 and may not work with earlier (or later) versions.

Installation
------------

To install the libraries, you need to place them into your "libraries" folder. You can find it within your Arduino IDE distribution within the "hardware" folder.

    C:\> cd [path to Arduino distribution]\libraries
    C:\> git clone https://github.com/stonehippo/arduino-ethernet ArduinoEthernet

Be sure to restart the IDE if it was running.

On a Mac, you will want to create a folder named "libraries" in in the "Documents" -> "Arduino" folder within your home directory. Clone the project there (and restart the IDE, if it was running during this process).

    $ cd ~/Documents/Arduino/libraries
    $ git clone https://github.com/stonehippo/arduino-ethernet ArduinoEthernet

Additional information can be found on the Arduino website: http://www.arduino.cc/en/Hacking/Libraries

Documentation
-------------

Documentation for the individual libraries is available here: http://gkaindl.com/software/arduino-ethernet