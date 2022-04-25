#ifndef RDM_RdmThread_hh
#define RDM_RdmThread_hh

//
// static a volatile prameters for running the RDM
//

#include "RDM/inc/RdmControls.hh"
#include <ctime>
#include <string>

namespace mu2e {

class RdmThread {
 public:
  enum thtype { discovery = 0, declare = 1, copy = 2, record = 3, clean = 4 };

  RdmThread(RdmControls& controls, thtype type, size_t copyNo, size_t index);
  virtual ~RdmThread() = default;

  RdmConfig& config() { return _controls._config; }
  RdmLogger& logger() { return _controls._logger; }
  RdmStat& stat() { return _controls._stat; }
  RdmCache& cache() { return _controls._cache; }

  thtype type() const { return _type; }
  size_t copyNo() const { return _copyNo; }
  size_t index() const { return _index; }
  const std::string& source() const { return _source; }
  std::time_t heartbeat() const { return _heartbeat; }

  void beat() { _heartbeat = time(nullptr); }

  virtual void execute() = 0;

 private:
  RdmControls& _controls;
  thtype _type;
  size_t _copyNo;
  size_t _index;
  std::string _source;  // for messages

  int _command;
  int _action;
  std::time_t _heartbeat;
};

}  // namespace mu2e

#endif
