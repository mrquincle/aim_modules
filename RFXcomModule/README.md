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

## What do I need to buy?

You will need to buy a RFxcom [Transmitter](http://www.rfxcom.com/store/Transmitters). We used the [RFXtrx433](RFXtrx433E USB 433.92MHz Transceiver) tranceiver which can send, receive, decode, and encode 433MHz signals for all major brands and connects to USB on your laptop, desktop, or Raspberry Pi. At [RFxcom Downloads](http://www.rfxcom.com/Downloads) you can find a user guide, and if you ask the creator of the device friendly, you can also get the SDK. You do not need the SDK to be able to talk with the devices we support, which on the moment are the HomeEasy, Klik-aan-Klik-uit, and Conrad home automation devices. If you want us to support something else, drop an email or file an issue.

## How to install?

This module does depend on a rule that gives a symbolic name to the RFXcom device. So create the file `/etc/udev/rules.d/75-rfxcom.rules` and add the following rule (you will need `sudo` rights for this).

    SUBSYSTEM=="tty", ENV{ID_MODEL}=="RFXtrx433", ENV{ID_VENDOR}=="RFXCOM", SYMLINK+="rfxcom",  MODE="0666"

Note that `udev` is the system that is used for device management on `Ubuntu` and the `Raspberry PI`, however for example MacOS does not use `udev`, so you will need to find another way to make sure this program can find the RFXcom device on `/dev/rfxcom`.

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## Usage as a die-hard developer

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

## Usage as a normal developer

First of all, you will need to do one technical thing and that is to install a so-called `docker` on your laptop. This is a sort of lightweight virtual machine, a linux container, you can read more on [docker.io](http://docker.io/). It allows you to run code within this virtual machine without being able to **touch** your bare metal machine. So, you can run our code in a docker on your server as well, without security implications! The instruction to run the docker can be found at [github](https://github.com/vliedel/docker).

It will be one big command of the form:
```bash
sudo docker run -e XMPP_NAMESERVER_USERNAME=$DODEDODO_USERNAME \
  -e XMPP_NAMESERVER_PASSWORD=$DODEDODO_PASSWORD \
  -e XMPP_NAMESERVER_RESOURCE=$DODEDODO_RESOURCE \
  -e AIM_WORKSPACE=/aim_workspace --interactive \
  --privileged -v /dev/rfxcom:/dev/rfxcom \
  -t dobots/aim_dodedodo bash
```

Where you have to set the environmental variables accordingly.

In the docker you run:
```bash
dodedodo
```

If you now go to [dodedodo](http://dodedodo.com) you will see a container with a raspberry painted in the corner:

![Screenshot Dodedodo](pictures/rfxcom_dodedodo.png?raw=true "Screenshot Dodedodo")

The `RFXcom` module can be found in the category `Home Automation`, we drop it on the box with the Raspberry. Now, we wire it. First, there is one `Integer` (category `General`) going in to specify the **type** of device. This is `0x10` for the Conrad switch (so `16` in decimal notation). It goes to the ingoing port `type0`. Second, we need an `IntegerArray` (also category `General`). It goes to ingoing port `select0` and we use it to indicate the **switch number** we want to control. We set it to `1` which means that the two dials are set to `I` and `1`. Third, we need another `IntegerArray` and set this again to `1` and connect it to `input0`. This is the value we send to the switch: `1` means on, `0` means off.

We now run the entire circus by pressing `Start`. It's now fun to look into your docker container and see all commands that are executed. The process is really transparent. You see code downloaded from github, things compiled, the module run, and commands being send to it. And voilá, the light is switched on! To turn off the light, you will have to send a `0` to `input0` which you now know how to do via the web interface.

## Future

In the future we will add more devices to the `RFXcom`, but this does not have our highest priority. We need to plet a few bugs in streamlining this process. For example, you might need to send a few times the `Start` command, because messages arrive out of order. It is good if a module does not depend on the order of the messages it gets. Moreover, the current setup of Dodedodo is fully asynchronous, which requires the receiving party to be able to buffer the commands its get. The code in the docker is not able to do that properly yet. And for example, when a github repository download is very slow, a new command can be tried to be executed, while a previous one is still pending.

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 
* [Dodedodo](http://dodedodo.com) 
* [DoBots](http://dobots.nl) 

## Copyrights
The copyrights of this module (2014) belong to:

- Author: Anne C. van Rossum
- Distributed Organisms B.V.

