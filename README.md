<!-- Uses markdown syntax for neat display at github -->

# AIM Modules of MrQuincle

The internet-of-devices infrastructure, [Dodedodo](http://www.dodedodo.com/), allows software developers to incorporate their own business logic in between devices. So, a camera can be connected to a motion detection module in the cloud. The architecture is explained at [dobots.github.io/aim](https://dobots.github.io/aim/).

This repository contains modules that are of interest to me. They are always tested with the YARP middleware, and through Dodedodo they also can be used with ZeroMQ, ROS, or Android intents. 

Currently, there are several types of modules here, across the dimensions "Devices", "Machine Learning", and "Computer Vision".

### Devices

* HorizonModule, to connect with the UPC Horizon box
* RFXcomModule, to connect with the RFXcom modules for 433MHz home automation devices
* BlueSourceModule, to connect with an Arduino using Bluetooth

### Machine Learning

* DatasetModule, generates datasets that can be used for classification / regression tasks (for testing purposes)
* MovingAverageModule, as simple as it is, an average that is incrementally updated with new data
* ClusterModule, a module that performs unsupervised learning with either k-means clustering or a Gaussian Mixture Model with Expectation Maximization
* ILVQModule, a different clustering method using Incremental Learning Vector Quantization
* NaiveBayes, a Naive Bayes classifier (assumes conditional independence between observations without any additional structure)
* LDAModule, a classifier that uses Latent Dirichlet Allocation (assumes one level of indirection: a hidden layer of topics), uses Gibbs sampling to perform inference
* ModelSectionModule, simple module that calculates the Akaike Information Criterion or the Bayesian Information Criterion which both penalize the model complexity and are therefore useful as model selection criteria
* BeliefModule, module which performs belief propagation (not fully done, uses only example, not input/output ports yet)
* DirichletModule, implements the Hierarchical Dirichlet Process through the Chinese Restaurant metaphor (not completed)

### Computer Vision

* KeypointModule, extraction of keypoints from an image
* CornerDetectorModule, detects corners in an image
* FindRobotModule, (not fully implemented!), uses Efficient Subwindow Search to match against a dictionary with image patches
* DetectLineModule, line detection in an image, uses the Randomized Hough Transfform

## Copyrights
The copyrights (2014) belong to:

- Author: Anne van Rossum
- Almende B.V., http://www.almende.com and DoBots, http://www.dobots.nl
- Rotterdam, The Netherlands
