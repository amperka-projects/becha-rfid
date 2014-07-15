===
BECHA RFID tape
===

Installation
============

Preparing Raspberry Pi:
* Burn Wolfson's Linux image to SD card
* Insert SD into Raspberry Pi, connect board to net
* Login to RPi by SSH (default login: pi, password: raspberry)
* Install Ruby by invoking command `apt-get install ruby`
* Clone that repository (`git clone https://github.com/amperka-projects/becha-rfid`)
* `cd ./becha-rfid`
* Record tracks: `./becha-server record <path-to-track>`
* Start server: `./becha-server power-on`
* To stop server: `./becha-server power-off`

Preparing RFID remote control:
* TODO: Hello, Vasya!

Requirements
============

* 8 GB SD card
* Raspberry Pi B
* Arduino Leonardo
* WiFi-shield for Arduino
* RFID-module
* Wolfson Pi Audio Card [optional]

Getting Help
============

* Take a look at `Wiki page <http://wiki.amperka.ru/becha-rfid>`_.
* Help for BECHA-server provided by ``becha-server --help``.
* Post `issues to GitHub <http://github.com/amperka-projects/becha-rfid/issues>`_.

License
=======

BECHA-rfid is distributed in terms of MIT software license.

Contributors
============

* `Vasiliy ACos <https://github.com/acosinwork>`_.
* `Artem ALev Levenkov <https://github.com/levenkov>`_.

Changelog
=========

1.0.0 (15.07.14)
    * Basic functionality done
