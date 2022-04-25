#ifndef RDM_RdmCopy_hh
#define RDM_RdmCopy_hh

//
// operating as a thread, poll the cache for a slice of files
// which have been declared, but not copied, and copy them
// the records are updated with a copy time or an error time
//

#include "RDM/inc/RdmThread.hh"
#include <atomic>
#include <ctime>
#include <string>

namespace mu2e {

class RdmCopy : public RdmThread {
 public:
  RdmCopy(RdmControls& controls, size_t copyNo, size_t index) :
      RdmThread(controls, thtype::copy, copyNo, index) {}

  void execute() override;

 private:
  void initialize();
  void copy(RdmFileSlice::ptr_t& ptr);
  void simCopy(RdmFileSlice::ptr_t& ptr);
};

}  // namespace mu2e

#endif
