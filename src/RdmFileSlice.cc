#include "RDM/inc/RdmFileSlice.hh"
#include <ctime>

// std::shared_lock lock(_mutex);
// std::unique_lock lock(_mutex); // write lock

using namespace mu2e;

//***********************************************************

void RdmFileSlice::lock(size_t threadIndex) {
  _locked = true;
  _threadIndex = threadIndex;
  _lockTime = std::time(nullptr);
}

//***********************************************************

void RdmFileSlice::unlock() {
  _locked = false;
  _threadIndex = std::numeric_limits<int>::max();
  _lockTime = 0;
}
