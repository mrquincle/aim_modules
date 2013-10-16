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
	
	Equations used:
	- AIC=2*k-2*ln(L)
	- AICc=2*k-2*ln(L)+\frac{2*k*(k+1)}{n-k-1}
	- BIC=-2*ln(L)+k*ln(n)
	- L_{max} \in \underset{\theta}{\operatorname{arg}\!\operatorname{max}} p(x|\theta,M)
-->

# ModelSelectionModule

## What does it do?

The ModelSelectionModule performs model selection: it picks a model from a set of candidate models. The quality of a model (compared to the others) can be measured by certain criteria:

* The [Akaike Information Criterion](https://en.wikipedia.org/wiki/Akaike_information_criterion) calculates AIC (see below), with k the number of parameters and L the maximum of the likelihood function for the given model. The preferred model is the one with the lowest complexity, that is the one with the lowest AIC value. To adjust for sample size, AICc (see below) is recommended, with n the sample size.
* The [Bayesian Information Criterion](https://en.wikipedia.org/wiki/Bayesian_information_criterion) penalizes model complexity even more (see below). Here L is again the maximal value of the likelihood function. 

### Equations

The equations might not be fun to everone, but for completeness sake, they're here:

![equation](http://latex.codecogs.com/gif.latex?AIC%3D2*k-2*ln%28L%29)
![equation](http://latex.codecogs.com/gif.latex?AICc%3D2*k-2*ln%28L%29%2B%5Cfrac%7B2*k*%28k%2B1%29%7D%7Bn-k-1%7D) 
![equation](http://latex.codecogs.com/gif.latex?BIC%3D-2*ln%28L%29%2Bk*ln%28n%29)
![equation](http://latex.codecogs.com/gif.latex?L_%7Bmax%7D+%5Cin+%5Cunderset%7B%5Ctheta%7D%7B%5Coperatorname%7Barg%7D%5C%21%5Coperatorname%7Bmax%7D%7D+p%28x%7C%5Ctheta%2CM%29)

## How fast is it?

The ModelSelectionModule ...

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim-bzr/). 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim-bzr/) 
* [WP: Model Selection](https://en.wikipedia.org/wiki/Model_selection)

## Copyrights
The copyrights of this module (2013) belong to:

- Author: Your Name
- Your company or institute
- Your address

