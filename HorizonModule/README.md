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

# HorizonModule

## What does it do?

The HorizonModule allows you to control the UPC Horizon box. It can switch channels, etc. A list of the commands can be found at [keys.hpp](https://github.com/mrquincle/aim_modules/blob/master/HorizonModule/inc/keys.hpp).

## How to install?

For best user experience, it is wise to follow the instructions on the [AIM website](http://dobots.github.com/aim/). However, I will describe the minimum requirements here. You will need one type of **middleware** to talk to the module. Install for example [YARP](https://github.com/robotology/yarp) or via the [PPA](https://code.launchpad.net/~yarpers/yarp/github-import). 

* cd HorizonModule
* aimselect yarp
* make
* aimregister
* yarp server # start first the server
* aimrun 0 # and run now the module

Because we use YARP, we can use the browser to send data to this module; browse to http://127.0.0.1:10000 (or wherever your YARP server is running). The output of aimrun 0 is in my case:

    yarp: Port /horizonmodule0/channel active at tcp://127.0.0.1:10002
    yarp: Port /horizonmodule0/command active at tcp://127.0.0.1:10003
    yarp: Port /horizonmodule0/address active at tcp://127.0.0.1:10004

Make sure you can see the Horizon box (assuming you are wireless and have assigned yourself an IP address in the 192.168.192.xxx range):

    sudo arp-scan -l -I wlan0
    Interface: wlan0, datalink type: EN10MB (Ethernet)
    Starting arp-scan 1.8.1 with 256 hosts (http://www.nta-monitor.com/tools/arp-scan/)
    192.168.192.32	3c:62:00:6e:6f:b9	Samsung electronics CO., LTD
    192.168.192.33	3c:62:00:6e:6f:bc	Samsung electronics CO., LTD

If you do not see these, you can use the settings on the Horizon box itself to find out which IP address it has. From the IP addresses above you want to talk to the one running the VNC server, so run nmap:

    nmap -sV 192.168.192.33
    PORT      STATE SERVICE    VERSION
    80/tcp    open  tcpwrapped
    1234/tcp  open  http       lighttpd 1.4.28
    5900/tcp  open  vnc        VNC (protocol 3.8)
    49153/tcp open  unknown

This is the correct device, supporting VNC over the default 5900 port. Now, we can fill in this address by sending data to the "/horizonmodule0/address" port (see http://127.0.0.1:10000/data=list again).

![picture](https://raw.github.com/mrquincle/aim_modules/master/HorizonModule/doc/yarp_address.png)

And now we can send commands. Send for example the number 7 to "/horizonmodule0/channel" and the Horizon box will change to RTL7.

You can also send commands over the "/horizonmodule0/command" port. A list of all possible commands can be found at [keys.hpp](https://github.com/mrquincle/aim_modules/blob/master/HorizonModule/inc/keys.hpp).

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) about the cloud computing framework of which this is just one component
* [Dodedodo website](http://dodedodo.com) about opening up this cloud computing software to everybody
* [Horizon box](https://github.com/mrquincle/horizon) about the Horizon box, a bit reverse engineered

## Copyrights
The copyrights of this module (2013) belong to:

- Author: Anne C. van Rossum
- Distributed Organisms B.V.

and are licensed under LGPLv3.

The copyrights of the code interfacing with the box itself (see the inc directory) belong to:

- Author: Rob Groenendijk

and are licensed under the Apache License 2.0.
