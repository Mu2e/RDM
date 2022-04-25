
#include "RDM/inc/RdmClean.hh"
#include <chrono>
#include <iostream>
#include <thread>

using namespace mu2e;

//***********************************************************

void RdmClean::execute() {
  beat();
  initialize();

  while (true) {
    if (config().verbose() > 4) {
      logger().print("loop", source());
    }
    beat();

    RdmFileSlice::ptr_t ptr =
        cache().getSlice(index(), RdmFileSlice::sstate::recorded);

    if (ptr) {
      if (config().verbose() > 4) {
        size_t n = ptr->files().size();
        std::string message("received slice of ");
        message += std::to_string(n);
        message += " for total ";
        message += std::to_string(n + _files.size());
        logger().print(message, source());
      }

      // this thread owns this ptr and the files now
      // copy the files into the internal file list
      // a little unusual pattern since itis unique_ptrs
      auto& inv = ptr->files();
      size_t nbase = _files.size();
      size_t nadd = inv.size();
      _files.resize(nbase + nadd);
      for (size_t i = 0; i < nadd; i++) {
        _files[nbase + i].swap(inv[i]);
      }

      // delete the empty cache slice object
      ptr.reset();

      auto dt = difftime(std::time(nullptr), _lastClean);
      if (dt > config().cleanInterval()) {
        if (config().simDisk()) {
          simClean(ptr);
        } else {
          clean(ptr);
        }
      }

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

void RdmClean::initialize() {
  _lastClean = std::time(nullptr);

  if (config().verbose() > 0) {
    logger().print("initialized", source());
  }
}

//***********************************************************

void RdmClean::clean(RdmFileSlice::ptr_t& ptr) {
  _lastClean = std::time(nullptr);
  return;
}

//***********************************************************

void RdmClean::simClean(RdmFileSlice::ptr_t& ptr) {
  _files.clear();
  if (config().verbose() > 3) {
    std::string message("deleted ");
    message += std::to_string(_files.size());
    message += " files";
    logger().print(message, source());
  }
  _lastClean = std::time(nullptr);
  return;
}
