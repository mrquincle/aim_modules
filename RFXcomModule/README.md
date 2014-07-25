<!-- Uses markdown syntax for neat display at github. This is the most important thing to your user. Be not afraid that
	you are too long-winded. If you tell someone what the Battacharyya distance is, they probably will appreciate
	that even if they already know. Be also clear about its complexity, say if it is exponential in time or the 
	number of pixels for example. 

	Tips on syntax:
	
	Use pictures:
	  ![picture](https://raw.github.com/git_username/git_repos/master/module_name/some_doc_folder/picture.jpg)

	Use math notation (http://stackoverflow.com/questions/11256433):
	- Experiment on http://latex.codecogs.com/gif.latex?c=\sqrt{E/m} to check your equation
	- Encode the math part c=\sqrt{E/m} on http://www.url-encode-decode.com/urlencode
	- And write it in markdown syntax as:
	   ![equation](http://latex.codecogs.com/gif.latex?c%3D%5Csqrt%7BE%2Fm%7D)
-->

# RFXcomModule

## What does it do?

The RFXcomModule opens up devices that are connected through an [RFXcom](http://www.rfxcom.com/) gateway. This module opens up four devices to be read or written via the gateway. There is no such limitation in the hardware, although of course it is true that the more devices are lingering around, the more contention there will be in the wireless spectrum.

The modules opens up four ports per device. One for sending it commands, one for getting its sensor values, one for selecting the device type, and one for selecting the device id. There will be several scenarios written for the Dodedodo platform using this module:

* Turning on/off a Conrad power switch depending on a ELRO motion detection sensor
* Finding the id of the Conrad power switch by plugging a lamp in it, and watching the result with a AvgBrightnessModule with a threshold

## How to install?

This module does depend on a rule that gives a symbolic name to the RFXcom device. So create the file `/etc/udev/rules.d/75-rfxcom.rules` and add the following rule (you will need `sudo` rights for this).

    SUBSYSTEM=="tty", ENV{ID_MODEL}=="RFXtrx433", ENV{ID_VENDOR}=="RFXCOM", SYMLINK+="rfxcom",  MODE="0666"

Note that `udev` is the system that is used for device management on `Ubuntu` and the `Raspberry PI`, however for example MacOS does not use `udev`, so you will need to find another way to make sure this program can find the RFXcom device on `/dev/rfxcom`.

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## Example

Suppose you have bought the Conrad remote-controlled switch with a label at the back that states RSL366R. This is the type `MSG_LIGHTING1`.

Run `./run.sh` in the `scripts` directory. Note that if this is the first time you use the RFXcom, you might to have a different mode. You can set the mode by enabling the `SetMode(..)` call in the source code in `src/RFXcomModuleExt.cpp`, but this will be made easier in the future. You can also try to run the `RFXmngr` utility from [rfxcom.com](http://www.rfxcom.com/downloads.htm), through `mono RFXmngr.exe`, and set the mode yourself, but I wasn't successful in writing (already reading went smoothly).

Now to control it, you have to configurate it first:

    yarp write /write /rfxcommodule0/type0
    type "16" which corresponds to 0x10 (MSG_LIGHTING1)

Quit the process (Ctrl+C) and start a new one:

    yarp write /rfxcommodule0/select0
    type "1" this corresponds to the rotating wheels on the back set to "I" and "1"

Quit again.

Start another:

    yarp write /rfxcommodule0/input0
    type "1" to turn on the switch
    type "0" to turn off the switch

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 

## Copyrights
The copyrights of this module (2014) belong to:

- Author: Anne C. van Rossum
- Distributed Organisms B.V.

