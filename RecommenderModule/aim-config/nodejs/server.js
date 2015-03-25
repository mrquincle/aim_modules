var nodeModule = require('./build/Release/RecommenderModule');

var obj = new nodeModule.RecommenderModule("0");

var recommendationCallBack = function(msg){
	check(msg);
};
obj.RegReadRecommendation(recommendationCallBack);

var train = function() {
	// List of document
	var doc0 = ["doc0", "test", "test", "test", "rest", "pest"];
	var doc1 = ["doc1", "test", "zest", "rest"];

	// Write documents to ports
	obj.WriteDocument(doc0);
	obj.WriteDocument(doc1);

	test();
}

var test = function() {
	var termQuery = ["doc0", "test"];
	obj.WriteTerm(termQuery);
}

var check = function(result) {
	console.log("recommendation:");
	console.log(result);
	
	stop();
}

var stop = function() {
	setTimeout(function(){obj.Destroy()},3000);
}

// Give the addon some time to process all training sets
setTimeout(train, 1000);

