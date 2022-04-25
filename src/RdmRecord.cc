
#include "RDM/inc/RdmRecord.hh"
#include <chrono>
#include <iostream>
#include <thread>

using namespace mu2e;

//***********************************************************

void RdmRecord::execute() {
  beat();
  initialize();

  while (true) {
    if (config().verbose() > 4) {
      logger().print("loop", source());
    }
    beat();

    // get an empty slice from the cache
    RdmFileSlice::ptr_t ptr =
        cache().getSlice(index(), RdmFileSlice::sstate::copied);

    if (ptr) {
      if (config().verbose() > 4) {
        size_t n = ptr->files().size();
        logger().print("received slice of " + std::to_string(n), source());
      }

      if (config().simDatabase()) {
        simDatabase(ptr);
      } else {
        database(ptr);
      }
      // these files are now recorded
      ptr->setState(RdmFileSlice::sstate::recorded);
      // done, release this slice
      cache().returnSlice(ptr);
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (config().simDelay() > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(config().simDelay()));
    }
  }
  return;
}

//***********************************************************

void RdmRecord::initialize() {
  if (config().verbose() > 0) {
    logger().print("initialized", source());
  }
}

//***********************************************************

void RdmRecord::database(RdmFileSlice::ptr_t& ptr) { return; }

//***********************************************************

void RdmRecord::simDatabase(RdmFileSlice::ptr_t& ptr) { return; }
