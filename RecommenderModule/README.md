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

The RecommenderModule only calculates sums of terms in documents and should be blazingly fast. To make it even faster remove debugging output. An implementation with unsigned ints as identifiers (rather than strings) would also be faster naturally.

## How to install?

Follow the instructions on [AIM website](http://dobots.github.com/aim/). 

## How to use?

There are currently three implementations tested:

### YARP

A YARP implementation is available:

```
# Start YARP server
$terminal -x yarp server 

# Start Module
$terminal -x ../builds/yarp/RecommenderModule 0

# Write first document
yarp write /write verbatim /recommendermodule0/document <<< "doc0 yes yes no maybe"

# Write second document
yarp write /write verbatim /recommendermodule0/document <<< "doc1 yes yes yes yes maybe perhaps"

# Open up a terminal to read the result
$terminal -x yarp read /read verbatim /recommendermodule0/recommendation

# Send a query
yarp write /write verbatim /recommendermodule0/term <<< "doc0 yes"

```

### Nodejs

There is also a NodeJS implementation available, see [standalone](aim-config/nodejs/standalone) in the `aim-config/nodejs` folder. 

    node server.js

### Eve

And there is an [Eve](https://github.com/enmasseio/evejs) implementation which allows you to use the module in this multi-agent system.

This implementation can be found in [eve](aim-config/nodejs/eve) in the `aim-config/nodejs` folder. 

```
# Start recommendation agent
node start.js

# Start request agent
node request.js
```

Of course you can adjust the request accordingly. See also the [Eve](https://github.com/enmasseio/evejs) documentation to find out how to use it from the browser. 

## Where can I read more?

* [AIM website](http://dobots.github.com/aim/) 

## Copyrights
The copyrights of this module (2015) belong to:

- Author: Anne van Rossum
- Copyrights: Distributed Organisms B.V. (http://dobots.nl)

