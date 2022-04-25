#include "RDM/inc/RdmCache.hh"
#include <iomanip>
#include <sstream>

// std::shared_lock lock(_mutex);
// std::unique_lock lock(_mutex); // write lock

using namespace mu2e;

//***********************************************************

RdmFileSlice::ptr_t RdmCache::getSlice(size_t threadIndex,
                                       RdmFileSlice::sstate state) {
  // write lock is created here, ends when it goes out of scope
  std::unique_lock lock(_mutex);
  // if request is for discovery, then this is the start of the process
  // and the cacheItem needs to be created

  if (state == RdmFileSlice::sstate::empty) {
    auto ptr = std::make_shared<RdmFileSlice>(_nextCacheId++, state, 0);
    ptr->lock(threadIndex);
    _items.push_front(ptr);
    if (_config.verbose() > 4) {
      _logger.print("created new empty slice", "cache");
    }
    return ptr;
  }

  if (_config.verbose() > 4) {
    std::string message("looking for state ");
    message += std::to_string(state);
    message += " for thread ";
    message += std::to_string(threadIndex);
    _logger.print(message, "cache");
  }

  // search for a matching and available cacheItem
  auto ibeg = _items.begin();
  auto it = _items.end();
  while (it != ibeg) {
    --it;  // reversed, so a FIFO
    RdmFileSlice::ptr_t ptr = *it;
    RdmFileSlice& fs = *ptr;
    if (fs.state() == state && !fs.locked()) {
      if (state == RdmFileSlice::sstate::recorded) {
        // these are passed to control of the clean thread
        // ok to destroy the entry since it is held shared with ptr
        _items.erase(it);
      } else {
        // these are locked until returned by the owning thread
        fs.lock(threadIndex);
      }
      return ptr;
    }
  }
  return RdmFileSlice::ptr_t();
}

void RdmCache::returnSlice(RdmFileSlice::ptr_t& ptr) {
  // write lock is created here, ends when it goes out of scope
  std::unique_lock lock(_mutex);
  ptr->unlock();
  if (_config.verbose() > 4) {
    std::string message("returned slice");
    _logger.print(message, "cache");
  }
  return;
}
