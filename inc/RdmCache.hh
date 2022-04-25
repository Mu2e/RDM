#ifndef RDM_RdmCache_hh
#define RDM_RdmCache_hh

//
// This class holds the slices of file lists as they are passed
// between the threads doing the various steps of the process
//

#include "RDM/inc/RdmConfig.hh"
#include "RDM/inc/RdmFileSlice.hh"
#include "RDM/inc/RdmLogger.hh"
#include "RDM/inc/RdmStat.hh"
#include <deque>
#include <limits>
#include <shared_mutex>
#include <string>
#include <vector>

namespace mu2e {

class RdmCache {
 public:
  RdmCache(RdmConfig& config, RdmLogger& logger, RdmStat& stat) :
      _config(config), _logger(logger), _stat(stat), _nextCacheId(0) {}

  // locate, return and lock a list of files
  // returns nullptr is nothing is available
  RdmFileSlice::ptr_t getSlice(size_t threadIndex, RdmFileSlice::sstate state);
  // a thread is done operating on the slice
  void returnSlice(RdmFileSlice::ptr_t& ptr);

 private:
  RdmConfig& _config;
  RdmLogger& _logger;
  RdmStat& _stat;

  std::deque<RdmFileSlice::ptr_t> _items;

  size_t _nextCacheId;  // a serial id for the cacheItems

  // multiple threads access this cache,
  // so lock it for each access
  mutable std::shared_mutex _mutex;
};

}  // namespace mu2e

#endif
