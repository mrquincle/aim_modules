var eve = require('evejs'); 
var recommenderAgent = require('./recommenderAgent');

// load configuration
var config = 
{ 
	transports: [ 
	{ 
		type:'local', 
		default: true 
	},
	{ 
		type: 'http', 
		url: 'http://127.0.0.1:3000/agents/:id',
		localShortcut: false
	} 
	] 
};
eve.system.init(config);

// create an agent
var agent = new recommenderAgent('recommenderAgent',0); 
console.log("Created Agent 'recommenderAgent'.");
