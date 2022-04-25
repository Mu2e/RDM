#ifndef RDM_RdmFileSlice_hh
#define RDM_RdmFileSlice_hh

//
// This class holds a slice of the file list and related bookkeeping info
//

#include "RDM/inc/RdmFile.hh"
#include <limits>
#include <string>
#include <vector>

namespace mu2e {

class RdmFileSlice {
 public:
  typedef std::shared_ptr<RdmFileSlice> ptr_t;

  enum sstate {
    empty = 0,
    discovered = 1,  // found on disk
    declared = 2,    // file record in db
    copied = 3,      // copied to dCache
    recorded = 3,    // copied recorded in db
    stored = 4,      // external process says on tape
    error = 5
  };  // in error

  RdmFileSlice(size_t id, sstate state, size_t nFile) :
      _id(id), _state(state), _files(nFile),
      _threadIndex(std::numeric_limits<int>::max()), _locked(false),
      _lockTime(0) {}

  size_t id() { return _id; }
  sstate state() { return _state; }
  RdmFile::vec_t &files() { return _files; }

  bool locked() { return _locked; }
  size_t threadIndex() { return _threadIndex; }
  time_t lockTime() { return _lockTime; }

  void setState(sstate s) { _state = s; }
  void lock(size_t threadIndex);
  void unlock();

 private:
  size_t _id;
  sstate _state;
  RdmFile::vec_t _files;

  size_t _threadIndex;
  bool _locked;
  time_t _lockTime;
};

}  // namespace mu2e

#endif
