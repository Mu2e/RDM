
#include "RDM/inc/RdmDeclare.hh"
#include <chrono>
#include <iostream>
#include <thread>

using namespace mu2e;

//***********************************************************

void RdmDeclare::execute() {
  beat();
  initialize();

  while (true) {
    if (config().verbose() > 4) {
      logger().print("loop", source());
    }
    beat();

    // get an empty slice from the cache
    RdmFileSlice::ptr_t ptr =
        cache().getSlice(index(), RdmFileSlice::sstate::discovered);

    if (ptr) {
      if (config().verbose() > 4) {
        logger().print(
            "received slice of " + std::to_string(ptr->files().size()),
            source());
      }

      if (config().simDisk()) {
        simDatabase(ptr);
      } else {
        database(ptr);
      }
      // these files are now declared
      ptr->setState(RdmFileSlice::sstate::declared);
      // done, release this slice
      cache().returnSlice(ptr);
    } else {
      // pause before trying the cache again
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    if (config().simDelay() > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(config().simDelay()));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    logger().print("in thread", "declare");
  }
  return;
}

//***********************************************************

void RdmDeclare::initialize() {
  beat();
  if (config().verbose() > 0) {
    logger().print("initialized", source());
  }
}

//***********************************************************

void RdmDeclare::database(RdmFileSlice::ptr_t& ptr) {}

//***********************************************************

void RdmDeclare::simDatabase(RdmFileSlice::ptr_t& ptr) {
  // just set a declare time
  auto tt = std::time(nullptr);
  for (auto& fp : ptr->files()) {
    fp->settDeclare(tt);
  }

  return;
}
