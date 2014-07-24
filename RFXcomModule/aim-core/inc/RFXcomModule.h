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

#ifndef RFXCOMMODULE_H_
#define RFXCOMMODULE_H_

#include <string>
#include <vector>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
// zeromq specific headers
#include <zmq.hpp>
#include <json_spirit_reader.h>
#include <pthread.h>


namespace rur {

struct Param {
  std::string module_id;
  int parameter;
};

typedef std::vector<int> long_seq;

/**
 * Port name service record. This is like a domain name service record, but instead of containing an IP address and an
 * URI, it comes with a "name" that can be resolved as a "host", "port", and "pid". The name is something like "/write",
 * the host something like "127.0.0.1" or "dev.almende.com" (that is resolvable by dns), "port" is a TCP/UDP port, and
 * "pid" is the process identifier.
 */
typedef struct pns_record_t {
  std::string name;
  std::string host;
  std::string port;
  std::string pid;
} pns_record;

// Following structure makes it easier to store state information per socket
typedef struct zmq_socket_ext_t {
  zmq::socket_t *sock;
  std::string name;
  bool ready;
} zmq_socket_ext;

class RFXcomModule {
private:
  Param *cliParam;
  
  // the socket over which is communicated with the name server
  zmq::socket_t *ns_socket;
  // standard control socket over which commands arrive to connect to some port for example
  zmq::socket_t *cmd_socket;
  pthread_t cmdThread;
  pthread_mutex_t cmdMutex;
  // standard control socket over which commands arrive to connect to some port for example
  std::vector<zmq_socket_ext*> zmq_sockets;
  long_seq portInput0Value;
  zmq_socket_ext portInput0In;
  
  int portType0Value;
  zmq_socket_ext portType0In;
  
  long_seq portSelect0Value;
  zmq_socket_ext portSelect0In;
  
  zmq_socket_ext portOutput0Out;
  
  long_seq portInput1Value;
  zmq_socket_ext portInput1In;
  
  int portType1Value;
  zmq_socket_ext portType1In;
  
  long_seq portSelect1Value;
  zmq_socket_ext portSelect1In;
  
  zmq_socket_ext portOutput1Out;
  
  long_seq portInput2Value;
  zmq_socket_ext portInput2In;
  
  int portType2Value;
  zmq_socket_ext portType2In;
  
  long_seq portSelect2Value;
  zmq_socket_ext portSelect2In;
  
  zmq_socket_ext portOutput2Out;
  
  long_seq portInput3Value;
  zmq_socket_ext portInput3In;
  
  int portType3Value;
  zmq_socket_ext portType3In;
  
  long_seq portSelect3Value;
  zmq_socket_ext portSelect3In;
  
  zmq_socket_ext portOutput3Out;
  
  static void* readCommandsHelper(void* object) {
    ((RFXcomModule*)object)->readCommands();
    return NULL;
  }
  
  void readCommands();
protected:
  static const int channel_count = 16;
  const char* channel[16];
  // the standard zeromq context object
  zmq::context_t *context;
  // some default debug parameter
  char debug;
  /**
   * The resolve function can be called by modules to get a new socket (and if you want host name and port). It can also
   * be used by the connector, to bind to these previously set up sockets.
   */
  void Resolve(pns_record & record);
  void SendAck(zmq::socket_t *s, bool state);
  bool ReceiveAck(zmq::socket_t *s, bool & state, bool blocking);
  char* GetReply(zmq::socket_t *s, bool & state, bool blocking, int & reply_size);
  void SendRequest(zmq::socket_t *s, bool & state, bool blocking, std::string str);
  void HandleCommand();
  void Connect(std::string source, std::string target);
  zmq::socket_t* GetSocket(std::string name);

public:
  // Default constructor
  RFXcomModule();
  
  // Default destructor
  virtual ~RFXcomModule();
  
  // Extend this with your own code, first call RFXcomModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  /**
   * The "readInput0" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput0(bool blocking=false);
  
  /**
   * The "readType0" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType0(bool blocking=false);
  
  /**
   * The "readSelect0" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect0(bool blocking=false);
  
  /**
   * The "writeOutput0" function sends stuff over a zeromq REQ socket. It works as a server. It cannot be blocking because this
   * would make it impossible to receive message on other ports (under which the /pid/control port). It could have been
   * blocking if it is known if it is connected to a REP port (but the connected() function is apparently not meant for
   * that).
   */
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput0(const int output0);
  
  /**
   * The "readInput1" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput1(bool blocking=false);
  
  /**
   * The "readType1" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType1(bool blocking=false);
  
  /**
   * The "readSelect1" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect1(bool blocking=false);
  
  /**
   * The "writeOutput1" function sends stuff over a zeromq REQ socket. It works as a server. It cannot be blocking because this
   * would make it impossible to receive message on other ports (under which the /pid/control port). It could have been
   * blocking if it is known if it is connected to a REP port (but the connected() function is apparently not meant for
   * that).
   */
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput1(const int output1);
  
  /**
   * The "readInput2" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput2(bool blocking=false);
  
  /**
   * The "readType2" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType2(bool blocking=false);
  
  /**
   * The "readSelect2" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect2(bool blocking=false);
  
  /**
   * The "writeOutput2" function sends stuff over a zeromq REQ socket. It works as a server. It cannot be blocking because this
   * would make it impossible to receive message on other ports (under which the /pid/control port). It could have been
   * blocking if it is known if it is connected to a REP port (but the connected() function is apparently not meant for
   * that).
   */
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput2(const int output2);
  
  /**
   * The "readInput3" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput3(bool blocking=false);
  
  /**
   * The "readType3" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType3(bool blocking=false);
  
  /**
   * The "readSelect3" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect3(bool blocking=false);
  
  /**
   * The "writeOutput3" function sends stuff over a zeromq REQ socket. It works as a server. It cannot be blocking because this
   * would make it impossible to receive message on other ports (under which the /pid/control port). It could have been
   * blocking if it is known if it is connected to a REP port (but the connected() function is apparently not meant for
   * that).
   */
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput3(const int output3);
  
};
} // End of namespace

#endif // RFXCOMMODULE_H_
