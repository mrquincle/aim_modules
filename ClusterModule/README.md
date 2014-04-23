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

# ClusterModule

## What does it do?

The ClusterModule is able to cluster data, also known as unsupervised learning. On the moment two methods are implemented:

* [k-means clustering](https://en.wikipedia.org/wiki/K-means_clustering)
* [Gaussian Mixture Model](https://en.wikipedia.org/wiki/Mixture_model#Gaussian_mixture_model) with as inference method Expectation-Maximization

The Gaussian Mixture Model works okay on simple methods such as a testset with only 2 or 3 Gaussians. However, it totally fails on more complex testsets, such as the Iris dataset. It is known that an initialization that places the Gaussians very far from their final destinations and close to each other, will take long to converge. Hence, it is recommended to first initialize the Gaussians using k-means for example! Note that the the covariance matrix is a generalisation of the variance (the standard deviation squared) over the variables taken into account (in this case the x and y coordinate in our 2D setting). 

So, take for example the `scripts/gaussian2_in_2d.m`, there is a variance in (x,y) of (0.1,0.1) for cluster A, and (0.1,0.5) for cluster B. The final covariance for the models is then for cluster A (0.01, 0; 0, 0.01) and cluster B (0.01, 0; 0, 0.25). Of course cluster A and B here can be swapped, there is no favorite index per cluster (or else it would have been a supervised learning task).

## How fast is it?

The ClusterModule uses [Eigen](http://eigen.tuxfamily.org/) for its matrix calculations and should be quite fast. However, there has been no specific attention to speed. There are many ways clustering can be accelerated, for example by hierarchical clustering. Henceforth, spending a lot of time on speeding up for example k-means clustering does not make much sense to me. I prefer to implement than a totally different method that can speed up everything with orders of magnitude instead taking into account special structure in the data (such as relational, ordinal, or hierarchical).

## How to install?

Follow the general instructions on [AIM website](http://dobots.github.com/aim/). 

Take note that this module uses Eigen. However, rather than having it as a system dependency, the code is included. This has as advantage that everything that is needed to compile this module is present (similar to the node.js/npm philosophy). This has as disadvantage that updates to Eigen are not automatically integrated with the code.

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 

## Copyrights
The copyrights of this module (2014) belong to:

- Author: Anne C. van Rossum
- Almende B.V.
- Westerstraat 50, Rotterdam, The Netherlands

