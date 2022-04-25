#ifndef RDM_RdmLogger_hh
#define RDM_RdmLogger_hh

//
// This simply prints messages to cout in a thread safe way
//

#include "RDM/inc/RdmConfig.hh"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <shared_mutex>
#include <string>

namespace mu2e {

class RdmLogger {
 public:
  RdmLogger(RdmConfig& config) : _config(config) {}
  void print(std::string message, std::string source = "");

 private:
  RdmConfig& _config;
  mutable std::shared_mutex _mutex;
};

}  // namespace mu2e

#endif
