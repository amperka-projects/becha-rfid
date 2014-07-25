===
BECHA RFID tape
===

Intro
====
![alt tag](https://github.com/amperka-projects/becha-rfid/blob/master/misc/becha-idea.png)


Installation
============

Preparing Raspberry Pi:
* Burn Wolfson's Linux image to SD card
* Insert SD into Raspberry Pi, connect board to net
* Login to RPi by SSH (default login: pi, password: raspberry)
* Install Ruby by invoking command `sudo apt-get install ruby`
* Clone that repository (`git clone https://github.com/amperka-projects/becha-rfid`)
* `cd ./becha-rfid`
* To install server:
  * `cd ./server`
  * `./prepare_wolfson`
  * `sudo gem install bechad`
* Record tracks: `bechad record <path-to-track>`
* Start server: `bechad start`
* To stop server: `bechad stop`
* To build RFID remote control:
  * !TODO!

Requirements
============

* ⩾4,5 GB SD card
* Raspberry Pi B
* Arduino Leonardo
* WiFi-shield for Arduino
* RFID-module
* Wolfson Pi Audio Card [optional]

Getting Help
============

* Take a look at `Wiki page <http://wiki.amperka.ru/becha-rfid>`.
* Help for BECHA-server provided by ``bechad --help``.
* Post `issues to GitHub <http://github.com/amperka-projects/becha-rfid/issues>`.

License
=======

BECHA-rfid is distributed in terms of MIT software license.

Contributors
============

* `Vasiliy ACos <https://github.com/acosinwork>`.
* `Artem ALev Levenkov <https://github.com/levenkov>`.

Changelog
=========

1.0.4 (25.07.14)
* becha-server renamed to bechad
* bechad pushed on rubygems, so you can install it by `gem install bechad`
* Bug fixes.

1.0.0 (15.07.14)
* Basic functionality done
