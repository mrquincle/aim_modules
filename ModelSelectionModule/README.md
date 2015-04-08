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
	- L_{max}=\underset{\theta}{\operatorname{max}}\{p(x|\theta,M)\}
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

![equation](http://latex.codecogs.com/gif.latex?L_%7Bmax%7D%3D%5Cunderset%7B%5Ctheta%7D%7B%5Coperatorname%7Bmax%7D%7D%5C%7Bp%28x%7C%5Ctheta%2CM%29%5C%7D)

## How fast is it?

The ModelSelectionModule ...

## Any critical notes or alternatives?

There are many information criterion around, such as the [Hannan-Quinn](https://en.wikipedia.org/wiki/Hannan%E2%80%93Quinn_information_criterion), [Deviance](https://en.wikipedia.org/wiki/Deviance_information_criterion), and [Focused](https://en.wikipedia.org/wiki/Focused_information_criterion) information criteria. These are however less widespread in use and are variants on a theme.

However, there is a more fundamental critique with respect to these information criteria [1]. They all punish parsimony: there should be as few parameters possible. And most of them establish fit by linear least squares. AIC for example is directly related to the residual sum of squares (RSS). The residuals are the discrepancies between the data points and the model. However, being small in a mathematical sense does not mean that those approximation errors are non-systematic. For purposes of induction we likely would approximate our data in such a way that we can predict, that is, we would like to have white noise left, not noise that is small, but with a significant structural component. The problem however, is that Spanos recommends mis-specification tests, more specifically [1]:

* Judicious combinations of parametric, nonparametric, omnibus, and simulation-based tests, probing as broadly as possible and invoking dissimular assumptions.
* Astute ordering of M-S tests so as to exploit the interrelationship among the model assumptions with a view to 'correct' each other's diagnosis.
* Joint M-S tests (testing several assumptions simultaneously) designed to avoid 'erroneous' diagnoses as well as minimize the maintained assumptions.

I have to admit, some of these I have no clue how I would start. But, apart from that, this seems widely inadequate for a good machine learning method. If model selection would release a battery of these tests, even in all kind of combinatorial combinations, a computer vision or robot algorithm would not make any progress.

It would be nice to have some simple tests that would cover most situations. For example, the runs test for randomness (IID), states that the expected number of runs (subsequent +'s or -'s in residuals) should be approximately (2n-1)/3. Using oscillation-type theorems this can be used to indicate that there is actually Markov dependence in the data and it's not IID after all. Tell me which tests I will need to run and I will be happy to implement them. :-)

[1] Akaike-type criteria and the reliability of inference: Model selection versus statistical model specification, Spanos, 2010, Journal of Econometrics

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim-bzr/) 
* [WP: Model Selection](https://en.wikipedia.org/wiki/Model_selection)

## Copyrights
The copyrights of this module (2013) belong to:

- Author: Your Name
- Your company or institute
- Your address

