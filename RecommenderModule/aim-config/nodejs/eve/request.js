var eve = require('evejs');
var agent = require('./requestAgent');


// configure eve
eve.system.init({ 
	transports: [ 
{ 
	type:'local', 
	default: true 
},
{ 
	type: 'http', 
	port: 3001,
   url: 'http://127.0.0.1:3000/agents/:id', 
	remoteUrl: 'http://127.0.0.1:3000/agents/:id',
	localShortcut: false
} 
] 
}); 


var requestAgent = new agent('agent0');

var doc0 = ["doc0", "test", "test", "test", "rest", "pest"];
var doc1 = ["doc1", "test", "rest", "zest"];
var doc2 = ["doc2", "zest"];

var query = ["doc0", "rest"];

// on training we don't get anything
console.log("Send train command");
var message = { method:'train', params: doc0 };
requestAgent.rpc.request('http://127.0.0.1:3000/agents/recommenderAgent', message);

console.log("Send train command");
message = { method:'train', params: doc1 };
requestAgent.rpc.request('http://127.0.0.1:3000/agents/recommenderAgent', message);

console.log("Send train command");
message = { method:'train', params: doc2 };
requestAgent.rpc.request('http://127.0.0.1:3000/agents/recommenderAgent', message);

// on our test we get a response
console.log("Send test query");
message = { method:'test', params: query };
requestAgent.rpc.request('http://127.0.0.1:3000/agents/recommenderAgent', message)
	.then(function(reply) {
		console.log("Recommendation: " + reply);
	})
	.catch(function(err) {
		console.log("Error message: " + err);
		console.log("Is the agent up and do you have a connection?");
	})
	;
