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

# RecommenderModule

## What does it do?

The RecommenderModule can be used as a recommender engine. It implements several algorithms from the literature. Note that it might also be worthwhile to look at different modules if you know the machine learning methods themselves. You can use for example Naive Bayes to build a recommender engine.

* TF-IDF
* ...

### TF-IDF

The algorithm [TF-IDF](http://en.wikipedia.org/wiki/Tf%E2%80%93idf) is very straightforward. TF stands for term frequency, the number of times a term is encountered in a document. We scale the result by log normalization as "1 + log f". IDF stands for inverse document frequency. This counts the number of documents that contains a specific term (and takes the inverse). A term might be relatively infrequent in a document, but if it is a very rare word in an overall corpus, it might still be of interest. It is common to pick "log (N / n_t)" and to prevent division by zero "log (N / n_t + 1)". The combined algorithm is a multiplication of both terms.



## How fast is it?

The RecommenderModule ...

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 

## Copyrights
The copyrights of this module (2015) belong to:

- Author: Homer J. Simpson
- Springfield Power Company

