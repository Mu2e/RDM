#ifndef RDM_RdmDeclare_hh
#define RDM_RdmDeclare_hh

//
// operating as a thread, poll the cache for a slice of files
// which have been discovered, but not declared, and declare them
// by making an entry in the database
//

#include "RDM/inc/RdmThread.hh"
#include <ctime>
#include <string>

namespace mu2e {

class RdmDeclare : public RdmThread {
 public:
  RdmDeclare(RdmControls& controls, size_t copyNo, size_t index) :
      RdmThread(controls, thtype::declare, copyNo, index) {}

  void execute() override;

 private:
  void initialize();
  void database(RdmFileSlice::ptr_t& ptr);
  void simDatabase(RdmFileSlice::ptr_t& ptr);
};

}  // namespace mu2e

#endif
