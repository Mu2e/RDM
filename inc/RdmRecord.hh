#ifndef RDM_RdmRecord_hh
#define RDM_RdmRecord_hh

//
// operating as a thread, poll the cache for a slice of files
// which have been copied, but the result is not recorded in the db,
// and record them
//

#include "RDM/inc/RdmThread.hh"
#include <atomic>
#include <ctime>
#include <string>

namespace mu2e {

class RdmRecord : public RdmThread {
 public:
  RdmRecord(RdmControls& controls, size_t copyNo, size_t index) :
      RdmThread(controls, thtype::record, copyNo, index) {}

  void execute() override;

 private:
  void initialize();
  void database(RdmFileSlice::ptr_t& ptr);
  void simDatabase(RdmFileSlice::ptr_t& ptr);
};

}  // namespace mu2e

#endif
