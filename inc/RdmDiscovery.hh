#ifndef RDM_RdmDiscovery_hh
#define RDM_RdmDiscovery_hh

//
// static a volatile prameters for running the RDM
//

#include "RDM/inc/RdmPriority.hh"
#include "RDM/inc/RdmThread.hh"
#include <atomic>
#include <ctime>
#include <string>

namespace mu2e {

class RdmDiscovery : public RdmThread {
 public:
  RdmDiscovery(RdmControls& controls, size_t copyNo, size_t index) :
      RdmThread(controls, thtype::discovery, copyNo, index),
      _priority(controls._config), _simFileNo(0) {}

  void execute() override;

 private:
  void initialize();
  void discover(RdmFileSlice::ptr_t& ptr);
  void simDisk(RdmFileSlice::ptr_t& ptr);

  RdmPriority _priority;
  size_t _simFileNo;  // for creating simulated files
};

}  // namespace mu2e

#endif
