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

# BlueSourceModule

## What does it do?

The BlueSourceModule receives data from the custom made Dotty robot at DO over Bluetooth and sends it to the free
sensor data accumulation platform CommonSense.

## How fast is it?

The BlueSourceModule is as slow as your Bluetooth connection. It is not fit for streaming images. It uses a ring
buffer which also adds to its latency. It is fast enough for data collection or to control the robot (eventually).

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim-bzr/). 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim-bzr/) 
* Dotty on [rest4phone](http://www.rest4phone.com) and [DO](http://www.dobots.nl) 

## Copyrights
The copyrights of this module (2013) belong to:

- Author: Anne van Rossum
- Almende B.V.
- Westerstraat 50, 3016 DJ, Rotterdam, The Netherlands

