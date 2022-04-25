
#include "RDM/inc/RdmDiscovery.hh"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace mu2e;

//***********************************************************

void RdmDiscovery::execute() {
  beat();
  initialize();

  while (true) {
    if (config().verbose() > 4) {
      logger().print("loop", source());
    }
    beat();

    // get an empty slice from the cache
    RdmFileSlice::ptr_t ptr =
        cache().getSlice(index(), RdmFileSlice::sstate::empty);

    if (config().verbose() > 4) {
      logger().print("received slice of " + std::to_string(ptr->files().size()),
                     source());
    }

    if (config().simDisk()) {
      simDisk(ptr);
    } else {
      discover(ptr);
    }

    if (config().verbose() > 4) {
      std::string message = std::string("created ") +
                            std::to_string(ptr->files().size()) + " files";
      logger().print(message, source());
    }

    // these files are now discovered
    ptr->setState(RdmFileSlice::sstate::discovered);
    // done, release this slice
    cache().returnSlice(ptr);

    if (config().simDelay() > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(config().simDelay()));
    }
  }  // continuous loop
  return;
}

//***********************************************************

void RdmDiscovery::initialize() {
  if (config().verbose() > 0) {
    logger().print("initializing", source());
  }
  // load up incomplete files from database
}

//***********************************************************

void RdmDiscovery::discover(RdmFileSlice::ptr_t &ptr) {}

//***********************************************************

void RdmDiscovery::simDisk(RdmFileSlice::ptr_t &ptr) {
  RdmFile::vec_t &v = ptr->files();
  std::string dir("/data/path");
  char fnc[100];

  size_t n;
  n = 20 + std::rand() % 30;

  for (size_t i = 0; i < n; i++) {
    snprintf(fnc, 100, "raw.mu2e.ce.v000.100000_%06lu.art", i + _simFileNo++);
    v.push_back(std::make_unique<RdmFile>(dir, fnc, _priority.priority(fnc)));
    if (config().verbose() > 9) {
      std::string message = std::string("created sim file ") + fnc;
      logger().print(message, source());
    }
  }

  n = 10 + std::rand() % 10;

  for (size_t i = 0; i < n; i++) {
    snprintf(fnc, 100, "raw.mu2e.cosmic.v000.100000_%06lu.art",
             i + _simFileNo++);
    v.push_back(std::make_unique<RdmFile>(dir, fnc, _priority.priority(fnc)));
    if (config().verbose() > 9) {
      std::string message = std::string("created sim file ") + fnc;
      logger().print(message, source());
    }
  }

  n = std::rand() % 5;

  for (size_t i = 0; i < n; i++) {
    snprintf(fnc, 100, "bck.mu2e.monitors.v000.100000_%06lu.tgz",
             i + _simFileNo++);
    v.push_back(std::make_unique<RdmFile>(dir, fnc, _priority.priority(fnc)));
    if (config().verbose() > 9) {
      std::string message = std::string("created sim file ") + fnc;
      logger().print(message, source());
    }
  }

  return;
}
