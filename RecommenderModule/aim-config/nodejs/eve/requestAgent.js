var eve = require('evejs');

function requestAgent(id, props) {
  // execute super constructor
  eve.Agent.call(this, id);

  this.props = props;

  // extend the agent with RPC functionality
  this.rpc = this.loadModule('rpc', this.rpcFunctions);

  // connect to all transports provided by the system
  this.connect(eve.system.transports.getAll());

}

// extend the eve.Agent prototype
requestAgent.prototype = Object.create(eve.Agent.prototype);
requestAgent.prototype.constructor = requestAgent;

requestAgent.prototype.rpcFunctions = {};

// we get a response on our request
requestAgent.prototype.rpcFunctions.answer = function (msg) {
	console.log("Answer from recommender: " + msg);
};

module.exports = requestAgent;
