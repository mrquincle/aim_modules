/**
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * @author               Anne C. van Rossum
 * @copyright            Distributed Organisms B.V.
 * @date                 Apr 18, 2014
 * @license              GNU General Lesser Public
 */

#include "RFXcomModule.h"

namespace rur {

RFXcomModule::RFXcomModule():
  ns_socket(NULL),
  cmd_socket(NULL),
  debug(0),
  cliParam(0)
{
  const char* const channel[16] = {"readInput0", "readType0", "readSelect0", "writeOutput0", "readInput1", "readType1", "readSelect1", "writeOutput1", "readInput2", "readType2", "readSelect2", "writeOutput2", "readInput3", "readType3", "readSelect3", "writeOutput3"};
  cliParam = new Param();
  context = new zmq::context_t(1);
  pthread_mutex_init(&cmdMutex, NULL);
  pthread_create(&cmdThread, 0, readCommandsHelper, this);
  portInput0In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portInput0In.ready = true;
  zmq_sockets.push_back(&portInput0In);
  portType0In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portType0In.ready = true;
  zmq_sockets.push_back(&portType0In);
  portSelect0In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portSelect0In.ready = true;
  zmq_sockets.push_back(&portSelect0In);
  portOutput0Out.sock = new zmq::socket_t(*context, ZMQ_REQ);
  portOutput0Out.ready = true;
  zmq_sockets.push_back(&portOutput0Out);
  portInput1In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portInput1In.ready = true;
  zmq_sockets.push_back(&portInput1In);
  portType1In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portType1In.ready = true;
  zmq_sockets.push_back(&portType1In);
  portSelect1In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portSelect1In.ready = true;
  zmq_sockets.push_back(&portSelect1In);
  portOutput1Out.sock = new zmq::socket_t(*context, ZMQ_REQ);
  portOutput1Out.ready = true;
  zmq_sockets.push_back(&portOutput1Out);
  portInput2In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portInput2In.ready = true;
  zmq_sockets.push_back(&portInput2In);
  portType2In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portType2In.ready = true;
  zmq_sockets.push_back(&portType2In);
  portSelect2In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portSelect2In.ready = true;
  zmq_sockets.push_back(&portSelect2In);
  portOutput2Out.sock = new zmq::socket_t(*context, ZMQ_REQ);
  portOutput2Out.ready = true;
  zmq_sockets.push_back(&portOutput2Out);
  portInput3In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portInput3In.ready = true;
  zmq_sockets.push_back(&portInput3In);
  portType3In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portType3In.ready = true;
  zmq_sockets.push_back(&portType3In);
  portSelect3In.sock = new zmq::socket_t(*context, ZMQ_REP);
  portSelect3In.ready = true;
  zmq_sockets.push_back(&portSelect3In);
  portOutput3Out.sock = new zmq::socket_t(*context, ZMQ_REQ);
  portOutput3Out.ready = true;
  zmq_sockets.push_back(&portOutput3Out);
}

RFXcomModule::~RFXcomModule() {
  delete cliParam;
  delete portInput0In.sock;
  delete portType0In.sock;
  delete portSelect0In.sock;
  delete portOutput0Out.sock;
  delete portInput1In.sock;
  delete portType1In.sock;
  delete portSelect1In.sock;
  delete portOutput1Out.sock;
  delete portInput2In.sock;
  delete portType2In.sock;
  delete portSelect2In.sock;
  delete portOutput2Out.sock;
  delete portInput3In.sock;
  delete portType3In.sock;
  delete portSelect3In.sock;
  delete portOutput3Out.sock;
}

void RFXcomModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::cout << "Connecting to name server..." << std::endl;
  ns_socket = new zmq::socket_t(*context, ZMQ_REQ);
  try {
    ns_socket->connect("tcp://127.0.0.1:10101"); // port to connect to, REQ/REP
  } catch (zmq::error_t & e) {
    std::cerr << "Error: Could not connect to the name server: = " << e.what() << std::endl;
  }
  cmd_socket = new zmq::socket_t(*context, ZMQ_REP);
  
  std::string zmqPortName;
  std::stringstream zmqSs;
  {
    pns_record record;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.name = "/resolve/" + zmqSs.str() + "/control";
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.clear(); zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port; 
    std::string zmqPortName = zmqSs.str(); 
    std::cout << "Bind to socket " << zmqPortName << std::endl; 
    cmd_socket->bind(zmqPortName.c_str());
  }
  
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portinput0";
    portInput0In.name = zmqPortName;
    zmqPortName = "/resolve" + portInput0In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portInput0In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/porttype0";
    portType0In.name = zmqPortName;
    zmqPortName = "/resolve" + portType0In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portType0In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portselect0";
    portSelect0In.name = zmqPortName;
    zmqPortName = "/resolve" + portSelect0In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portSelect0In.sock->bind(zmqPortName.c_str());
  }
  {
    // outgoing port, function as server
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portoutput0";
    portOutput0Out.name = zmqPortName;
    zmqPortName = "/resolve" + portOutput0Out.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portinput1";
    portInput1In.name = zmqPortName;
    zmqPortName = "/resolve" + portInput1In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portInput1In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/porttype1";
    portType1In.name = zmqPortName;
    zmqPortName = "/resolve" + portType1In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portType1In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portselect1";
    portSelect1In.name = zmqPortName;
    zmqPortName = "/resolve" + portSelect1In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portSelect1In.sock->bind(zmqPortName.c_str());
  }
  {
    // outgoing port, function as server
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portoutput1";
    portOutput1Out.name = zmqPortName;
    zmqPortName = "/resolve" + portOutput1Out.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portinput2";
    portInput2In.name = zmqPortName;
    zmqPortName = "/resolve" + portInput2In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portInput2In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/porttype2";
    portType2In.name = zmqPortName;
    zmqPortName = "/resolve" + portType2In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portType2In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portselect2";
    portSelect2In.name = zmqPortName;
    zmqPortName = "/resolve" + portSelect2In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portSelect2In.sock->bind(zmqPortName.c_str());
  }
  {
    // outgoing port, function as server
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portoutput2";
    portOutput2Out.name = zmqPortName;
    zmqPortName = "/resolve" + portOutput2Out.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portinput3";
    portInput3In.name = zmqPortName;
    zmqPortName = "/resolve" + portInput3In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portInput3In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/porttype3";
    portType3In.name = zmqPortName;
    zmqPortName = "/resolve" + portType3In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portType3In.sock->bind(zmqPortName.c_str());
  }
  {
    // incoming port, function as client
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portselect3";
    portSelect3In.name = zmqPortName;
    zmqPortName = "/resolve" + portSelect3In.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
    zmqSs.str("");
    zmqSs << "tcp://" << record.host << ":" << record.port;
    zmqPortName = zmqSs.str();
    std::cout << "Bind to socket " << zmqPortName << std::endl;
    portSelect3In.sock->bind(zmqPortName.c_str());
  }
  {
    // outgoing port, function as server
    zmqPortName = "/rfxcommodule" + cliParam->module_id + "/portoutput3";
    portOutput3Out.name = zmqPortName;
    zmqPortName = "/resolve" + portOutput3Out.name;
    pns_record record;
    record.name = zmqPortName;
    zmqSs.clear(); zmqSs.str("");
    zmqSs << getpid();
    record.pid = zmqSs.str();
    Resolve(record);
  }
}

void RFXcomModule::readCommands() {
  while (true) {
    HandleCommand();
  }
}


void RFXcomModule::Resolve(pns_record & record) {
  std::cout << "Acquire TCP/IP port for " << record.name << std::endl;
  std::string reqname = record.name + ':' + record.pid;
  zmq::message_t request (reqname.size() + 1);
  memcpy ((void *) request.data (), reqname.c_str(), reqname.size());
  ns_socket->send(request);
  
  zmq::message_t reply;
  if (!ns_socket->recv (&reply)) return;
  size_t msg_size = reply.size();
  char* address = new char[msg_size+1];
  memcpy (address, (void *) reply.data(), msg_size);
  address[msg_size] = '\0';
  std::string json = std::string(address);
  std::cout << "Received " << json << std::endl;
  delete [] address;
  
  // get json object
  bool valid;
  json_spirit::Value value;
  if (!json_spirit::read(json, value)) {
    valid = false;
    std::cerr << "Not a json value" << std::endl;
    return;
  }
  if (value.type() != json_spirit::obj_type) {
    std::cerr << "[1] Unexpected object type "" << Value_type_str[value.type()] << """;
    std::cerr << " instead of "" << Value_type_str[json_spirit::obj_type] << """ << std::endl;
    return;
  }
  
  // a "json_spirit object" is - by default - a vector of json pairs
  json_spirit::Object obj = value.get_obj();
  
  for( json_spirit::Object::size_type i = 0; i != obj.size(); ++i ) {
    const json_spirit::Pair& pair = obj[i];
    const std::string& key = pair.name_;
    const json_spirit::Value& value = pair.value_;
    if (key == "identifier") {
      // same thing
    } else if (key == "server") {
      record.host = value.get_str();
    } else if (key == "port") {
      record.port = value.get_str();
    } else if (key == "pid") {
      record.pid = value.get_str();
    }
  }
}

void RFXcomModule::SendAck(zmq::socket_t *s, bool state) {
  if (debug) std::cout << "Send ACK" << std::endl;
  SendRequest(s, state, true, "ACK");
}

bool RFXcomModule::ReceiveAck(zmq::socket_t *s, bool & state, bool blocking) {
  int reply_size = 0;
  char *reply = GetReply(s, state, blocking, reply_size);
  if (reply == NULL) return false;
  std::string req = std::string(reply);
  delete [] reply;
  if (req.find("ACK") != std::string::npos) {
    if (debug) std::cout << "Got ACK, thanks!" << std::endl;
    return true;
  }
  std::cerr << "Error: got \"" << req << "\", no ACK, state compromised" << std::endl;
  return false;
}
  
char* RFXcomModule::GetReply(zmq::socket_t *s, bool & state, bool blocking, int & reply_size) {
  if (s == NULL)
    return NULL;
  zmq::message_t reply;
  char* result = NULL;
  reply_size = 0;
  try {
    if (blocking)
      state = s->recv(&reply);
    else
      state = s->recv(&reply, ZMQ_DONTWAIT);
  } catch (zmq::error_t &e) {
    std::cout << "Error: received zmq::error_t " << e.what() << std::endl;
  }
  if (state) {
    size_t msg_size = reply.size();
    result = new char[msg_size+1];
    memcpy (result, (void *) reply.data(), msg_size);
    result[msg_size] = '\0';
    reply_size = msg_size;
    //std::cout << "Result: \"" << std::string(result) << "\"" << std::endl;
  }
  return result;
}

void RFXcomModule::SendRequest(zmq::socket_t *s, bool & state, bool blocking, std::string str) {
  if (state) {
    zmq::message_t request(str.size()+1);
    memcpy((void *) request.data(), str.c_str(), str.size()+1);
    if (debug) std::cout << "Send request: " << str << std::endl;
    if (blocking)
      state = s->send(request);
    else
      state = s->send(request, ZMQ_DONTWAIT);
  } else {
    std::cout << "Send nothing (still waiting to receive) " << std::endl;
  }
}

void RFXcomModule::HandleCommand() {
  int reply_size = -1;
  bool state = false;
  char *reply = GetReply(cmd_socket, state, true, reply_size);
  if (reply == NULL) return;
  std::cout << "HandleCommand: " << std::string(reply, reply_size) << std::endl;
  if (reply_size < 2) std::cerr << "Error: Reply is not large for magic header + command string" << std::endl;
  char magic_value = reply[0];
  reply[reply_size-1] = '\0';
  if (magic_value == 0x01) { // connect to command...
    std::string name = std::string(reply+1);
    int pos = name.find("->");
    if (pos == std::string::npos) {
      std::cerr << "Error: no -> separator in connect command" << std::endl;
      return;
    }
    std::string source = name.substr(0, pos);
    std::string target = name.substr(pos+2); // todo: 
    std::cout << "Connect from " << source << " to " << target << std::endl;
    Connect(source, target);
  } else {
    std::cerr << "Error: Unknown command!" << std::endl;
  }
  SendAck(cmd_socket, true);
  delete [] reply;
}

void RFXcomModule::Connect(std::string source, std::string target) {
  pthread_mutex_lock(&cmdMutex);
  zmq::socket_t *s = GetSocket(source);
  if (s == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return;
  }
  pns_record t_record;
  t_record.name = "/resolve" + target;
  Resolve(t_record);
  std::stringstream ss; ss.clear(); ss.str("");
  ss << "tcp://" << t_record.host << ":" << t_record.port; 
  std::string sock = ss.str(); 
  std::cout << "Connect to socket " << sock << std::endl; 
  try {
    s->connect(sock.c_str());
  } catch (zmq::error_t &e) {
    std::cerr << "Error: Could not connect to " << target << ", because: " << e.what() << std::endl;
  }
  pthread_mutex_unlock(&cmdMutex);
}

zmq::socket_t* RFXcomModule::GetSocket(std::string name) {
  for (int i = 0; i < zmq_sockets.size(); ++i) {
    if (zmq_sockets[i]->name.find(name) != std::string::npos) return zmq_sockets[i]->sock;
  }
  std::cerr << "Error: socket name could not be found! " << name << std::endl;
  return NULL;
  //assert(false); // todo, get the previously registered socket by name
}

long_seq* RFXcomModule::readInput0(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portInput0In.sock, portInput0In.ready, blocking, reply_size);
  if (!portInput0In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portInput0In.sock, portInput0In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portInput0Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portInput0Value.push_back(itemVal);
  }
  delete [] reply;
  return &portInput0Value;
}

int* RFXcomModule::readType0(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portType0In.sock, portType0In.ready, blocking, reply_size);
  if (!portType0In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portType0In.sock, portType0In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  ss >> portType0Value;
  delete [] reply;
  return &portType0Value;
}

long_seq* RFXcomModule::readSelect0(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portSelect0In.sock, portSelect0In.ready, blocking, reply_size);
  if (!portSelect0In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portSelect0In.sock, portSelect0In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portSelect0Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portSelect0Value.push_back(itemVal);
  }
  delete [] reply;
  return &portSelect0Value;
}

bool RFXcomModule::writeOutput0(const int output0) {
  std::stringstream ss; ss.clear(); ss.str("");
  ss << output0; // very dirty, no endianness, etc, just use the stream operator itself
  pthread_mutex_lock(&cmdMutex);
  bool state = portOutput0Out.ready;
  SendRequest(portOutput0Out.sock, state, false, ss.str());
  if (state) portOutput0Out.ready = false;
  if (!portOutput0Out.ready) {
    bool ack_state = true;
    ReceiveAck(portOutput0Out.sock, ack_state, true);
    if (ack_state) {
      portOutput0Out.ready = true;
      pthread_mutex_unlock(&cmdMutex);
      return true;
    }
  }
  pthread_mutex_unlock(&cmdMutex);
  return false;
}

long_seq* RFXcomModule::readInput1(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portInput1In.sock, portInput1In.ready, blocking, reply_size);
  if (!portInput1In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portInput1In.sock, portInput1In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portInput1Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portInput1Value.push_back(itemVal);
  }
  delete [] reply;
  return &portInput1Value;
}

int* RFXcomModule::readType1(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portType1In.sock, portType1In.ready, blocking, reply_size);
  if (!portType1In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portType1In.sock, portType1In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  ss >> portType1Value;
  delete [] reply;
  return &portType1Value;
}

long_seq* RFXcomModule::readSelect1(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portSelect1In.sock, portSelect1In.ready, blocking, reply_size);
  if (!portSelect1In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portSelect1In.sock, portSelect1In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portSelect1Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portSelect1Value.push_back(itemVal);
  }
  delete [] reply;
  return &portSelect1Value;
}

bool RFXcomModule::writeOutput1(const int output1) {
  std::stringstream ss; ss.clear(); ss.str("");
  ss << output1; // very dirty, no endianness, etc, just use the stream operator itself
  pthread_mutex_lock(&cmdMutex);
  bool state = portOutput1Out.ready;
  SendRequest(portOutput1Out.sock, state, false, ss.str());
  if (state) portOutput1Out.ready = false;
  if (!portOutput1Out.ready) {
    bool ack_state = true;
    ReceiveAck(portOutput1Out.sock, ack_state, true);
    if (ack_state) {
      portOutput1Out.ready = true;
      pthread_mutex_unlock(&cmdMutex);
      return true;
    }
  }
  pthread_mutex_unlock(&cmdMutex);
  return false;
}

long_seq* RFXcomModule::readInput2(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portInput2In.sock, portInput2In.ready, blocking, reply_size);
  if (!portInput2In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portInput2In.sock, portInput2In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portInput2Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portInput2Value.push_back(itemVal);
  }
  delete [] reply;
  return &portInput2Value;
}

int* RFXcomModule::readType2(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portType2In.sock, portType2In.ready, blocking, reply_size);
  if (!portType2In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portType2In.sock, portType2In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  ss >> portType2Value;
  delete [] reply;
  return &portType2Value;
}

long_seq* RFXcomModule::readSelect2(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portSelect2In.sock, portSelect2In.ready, blocking, reply_size);
  if (!portSelect2In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portSelect2In.sock, portSelect2In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portSelect2Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portSelect2Value.push_back(itemVal);
  }
  delete [] reply;
  return &portSelect2Value;
}

bool RFXcomModule::writeOutput2(const int output2) {
  std::stringstream ss; ss.clear(); ss.str("");
  ss << output2; // very dirty, no endianness, etc, just use the stream operator itself
  pthread_mutex_lock(&cmdMutex);
  bool state = portOutput2Out.ready;
  SendRequest(portOutput2Out.sock, state, false, ss.str());
  if (state) portOutput2Out.ready = false;
  if (!portOutput2Out.ready) {
    bool ack_state = true;
    ReceiveAck(portOutput2Out.sock, ack_state, true);
    if (ack_state) {
      portOutput2Out.ready = true;
      pthread_mutex_unlock(&cmdMutex);
      return true;
    }
  }
  pthread_mutex_unlock(&cmdMutex);
  return false;
}

long_seq* RFXcomModule::readInput3(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portInput3In.sock, portInput3In.ready, blocking, reply_size);
  if (!portInput3In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portInput3In.sock, portInput3In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portInput3Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portInput3Value.push_back(itemVal);
  }
  delete [] reply;
  return &portInput3Value;
}

int* RFXcomModule::readType3(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portType3In.sock, portType3In.ready, blocking, reply_size);
  if (!portType3In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portType3In.sock, portType3In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  ss >> portType3Value;
  delete [] reply;
  return &portType3Value;
}

long_seq* RFXcomModule::readSelect3(bool blocking) {
  pthread_mutex_lock(&cmdMutex);
  int reply_size = -1;
  char *reply = GetReply(portSelect3In.sock, portSelect3In.ready, blocking, reply_size);
  if (!portSelect3In.ready) {
    delete [] reply;
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  if (reply == NULL) {
    pthread_mutex_unlock(&cmdMutex);
    return NULL;
  }
  SendAck(portSelect3In.sock, portSelect3In.ready);
  pthread_mutex_unlock(&cmdMutex);
  if (reply_size < 2) std::cerr << "Error: Reply is not large enough to store a value!" << std::endl;
  std::stringstream ss; ss.clear(); ss.str("");
  ss << std::string(reply);
  int itemVal;
  std::stringstream ssItem;
  portSelect3Value.clear();
  std::string item;
  char delim(' ');
  while (std::getline(ss, item, delim)) {
    ssItem.clear(); ssItem.str("");
    ssItem << item;
    ssItem >> itemVal;
    portSelect3Value.push_back(itemVal);
  }
  delete [] reply;
  return &portSelect3Value;
}

bool RFXcomModule::writeOutput3(const int output3) {
  std::stringstream ss; ss.clear(); ss.str("");
  ss << output3; // very dirty, no endianness, etc, just use the stream operator itself
  pthread_mutex_lock(&cmdMutex);
  bool state = portOutput3Out.ready;
  SendRequest(portOutput3Out.sock, state, false, ss.str());
  if (state) portOutput3Out.ready = false;
  if (!portOutput3Out.ready) {
    bool ack_state = true;
    ReceiveAck(portOutput3Out.sock, ack_state, true);
    if (ack_state) {
      portOutput3Out.ready = true;
      pthread_mutex_unlock(&cmdMutex);
      return true;
    }
  }
  pthread_mutex_unlock(&cmdMutex);
  return false;
}

} // namespace
