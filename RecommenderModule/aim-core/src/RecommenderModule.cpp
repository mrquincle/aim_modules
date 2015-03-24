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
 * @author               Anne van Rossum
 * @copyright            Distributed Organisms B.V. (DoBots)
 * @date                 Mar 23, 2015
 * @license              LGPLv3
 */

#include "RecommenderModule.h"

namespace rur {
using namespace yarp::os;

RecommenderModule::RecommenderModule():
  cliParam(0)
{
  const char* const channel[3] = {"readDocument", "readTerm", "writeRecommendation"};
  cliParam = new Param();
  portDocument = new BufferedPort<Bottle>();
  portTerm = new BufferedPort<Bottle>();
  portRecommendation = new BufferedPort<Bottle>();
}

RecommenderModule::~RecommenderModule() {
  delete cliParam;
  delete portDocument;
  delete portTerm;
  delete portRecommendation;
}

void RecommenderModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/recommendermodule" << name << "/document";
  portDocument->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/recommendermodule" << name << "/term";
  portTerm->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/recommendermodule" << name << "/recommendation";
  portRecommendation->open(yarpPortName.str().c_str());
  
}

std::string* RecommenderModule::readDocument(bool blocking) {
  Bottle *b = portDocument->read(blocking);
  if (b != NULL) { 
    portDocumentBuf = "";
    for (int i = 0; i < b->size(); ++i) {
      portDocumentBuf.append(b->get(i).asString());
    }
    return &portDocumentBuf;
  }
  return NULL;
}

std::string* RecommenderModule::readTerm(bool blocking) {
  Bottle *b = portTerm->read(blocking);
  if (b != NULL) { 
    portTermBuf = "";
    for (int i = 0; i < b->size(); ++i) {
      portTermBuf.append(b->get(i).asString());
    }
    return &portTermBuf;
  }
  return NULL;
}

bool RecommenderModule::writeRecommendation(const std::string output) {
  Bottle &outputPrepare = portRecommendation->prepare();
  outputPrepare.clear();
  outputPrepare.addString(output);
  bool forceStrict = true; // wait till previous sends are complete
  portRecommendation->write(forceStrict);
  return true;
}

} // namespace
