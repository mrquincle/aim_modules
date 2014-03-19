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

# DatasetModule

## What does it do?

The DatasetModule implements the dataset as defined on the 
[Wikipedia](https://en.wikipedia.org/wiki/Naive_Bayes_classifier#Document_Classification) page on Naive Bayes classifiers.

Table:

    sex	        height        weight     foot size
    male        600           180        12
    male        592  (5'11")  190        11
    male        558  (5'7")   170        12
    male        592  (5'11")  165        10
    female      500           100        6
    female      550  (5'6")   150        8
    female      542  (5'5")   130        7
    female      575  (5'9")   150        9

The only difference is that the feet values are multiplied with a factor 100, so they are just integers.

Using a Gaussian assumption this results in the following sample variances:

    sex     mean (height)   variance (height)  mean (weight)      variance (weight)   mean (foot size)   variance (foot size)
    male    585.5           3.5033e+02         176.25             1.2292e+02          11.25              9.1667e-01
    female  541.75          9.7225e+02         132.5              5.5833e+02          7.5                1.6667e+00

The difference is a factor 100 in the average height, and a factor 10000 in the height variance.

Then with the given sample:

    sex            height   weight   foot size
    to determine   600      130      8

The conditional probabilities should be:

    p(height|male)      : 0.0157888
    p(height|female)    : 0.00223459
    p(weight|male)      : 5.98674e-06
    p(weight|female)    : 0.0167893
    p(foot size|male)   : 0.00131122
    p(foot size|female) : 0.286691

The product with a prior of `p(male)=p(female)=1/2` is:

    p(sex=male) = 6.19707e-11 
    p(sex=female) = 5.37791e-06 

So, overwhelmingly female.

## How fast is it?

This module is not build for speed at all. :-)

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 

## Copyrights
The copyrights of this module (2014) belong to:

- Author: Anne C. van Rossum
- Distributed Organisms B.V.

