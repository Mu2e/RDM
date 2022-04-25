
#include "RDM/inc/RdmCopy.hh"
#include <chrono>
#include <iostream>
#include <thread>

using namespace mu2e;

//***********************************************************

void RdmCopy::execute() {
  beat();
  initialize();

  while (true) {
    if (config().verbose() > 4) {
      logger().print("loop", source());
    }
    beat();

    // get an empty slice from the cache
    RdmFileSlice::ptr_t ptr =
        cache().getSlice(index(), RdmFileSlice::sstate::declared);

    if (ptr) {
      if (config().verbose() > 4) {
        size_t n = ptr->files().size();
        logger().print("received slice of " + std::to_string(n), source());
      }

      if (config().simDcache()) {
        simCopy(ptr);
      } else {
        copy(ptr);
      }
      // these files are now copied
      ptr->setState(RdmFileSlice::sstate::copied);
      // done, release this slice
      cache().returnSlice(ptr);
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (config().simDelay() > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(config().simDelay()));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return;
}

//***********************************************************

void RdmCopy::initialize() {
  beat();
  if (config().verbose() > 0) {
    logger().print("initialized", source());
  }
}

//***********************************************************

void RdmCopy::copy(RdmFileSlice::ptr_t& ptr) {}

//***********************************************************

void RdmCopy::simCopy(RdmFileSlice::ptr_t& ptr) {
  // just set a copy time
  auto tt = std::time(nullptr);
  for (auto& fp : ptr->files()) {
    fp->settDcache(tt);
  }

  return;
}
