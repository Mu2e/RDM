#ifndef RDM_RdmClean_hh
#define RDM_RdmClean_hh

//
// operating as a thread, watch for lmits on active disk space,
// and when needed, contact the database to ask for files which
// are on tape, and delete them from disk. Once deleted,
// also remove the db record
//

#include "RDM/inc/RdmThread.hh"
#include <atomic>
#include <ctime>
#include <string>

namespace mu2e {

class RdmClean : public RdmThread {
 public:
  RdmClean(RdmControls& controls, size_t copyNo, size_t index) :
      RdmThread(controls, thtype::clean, copyNo, index) {}

  void execute() override;

 private:
  void initialize();
  void clean(RdmFileSlice::ptr_t& ptr);
  void simClean(RdmFileSlice::ptr_t& ptr);

  // files waiting to be deleted
  RdmFile::vec_t _files;
  std::time_t _lastClean;
};

}  // namespace mu2e

#endif
