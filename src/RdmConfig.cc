#include "RDM/inc/RdmConfig.hh"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace mu2e;

// ***********************************************************

void RdmConfig::defaults() {
  _verbose = 1;
  _nSlice = 50;
  _sliceSize = 100;
  _nCopyThreads = 2;
  _cleanInterval = 100;
  _priorities = "*ele*.art = 10, *cosmic*.art = 8 , *.tgz = 3";
  _priority = 5;
  _simDisk = false;
  _simDatabase = false;
  _simDcache = false;
  _simDelay = 0;

  return;
}

// ***********************************************************

void RdmConfig::testMode() {
  defaults();
  _verbose = 5;
  _simDisk = true;
  _simDatabase = true;
  _simDcache = true;
  _simDelay = 5;

  return;
}

// ***********************************************************

std::string RdmConfig::toString() {
  std::ostringstream ss;
  ss << "RDM parameters:\n";
  ss << "    verbose      = " << _verbose << "\n";
  ss << "    nSlice       = " << _nSlice << "\n";
  ss << "    sliceSize    = " << _sliceSize << "\n";
  ss << "    nCopyThreads = " << _nCopyThreads << "\n";
  ss << "    priorities   = " << _priorities << "\n";
  ss << "    priority     = " << _priority << "\n";
  ss << "    node         = " << _node << "\n";
  ss << "    port         = " << _port << "\n";
  ss << "    database     = " << _database << "\n";
  if (_simDisk || _simDcache || _simDatabase) {
    ss << "    simDisk      = " << _simDisk << "\n";
    ss << "    simDatabase  = " << _simDatabase << "\n";
    ss << "    simDcache    = " << _simDcache << "\n";
    ss << "    simDelay     = " << _simDelay << "\n";
  }

  return ss.str();
}

//***********************************************************

int RdmConfig::readFile(std::string const& fn) {
  std::fstream myfile;
  myfile.open(fn);
  if (!myfile.is_open()) {
    return 1;
  }

  std::string line;
  while (std::getline(myfile, line)) {
    boost::trim(line);  // remove whitespace
    // line was blank or comment, skip to next
    if (line.size() <= 0 || line[0] == '#') continue;
    size_t icolon = line.find(':');
    if (icolon == std::string::npos) {
      return 1;
    }
    std::string name = line.substr(0, icolon);
    boost::trim(name);
    std::string value = line.substr(icolon + 1);
    boost::trim(value);

    if (name == "verbose") {
      _verbose = std::stoul(value);
    } else if (name == "sliceSize") {
      _sliceSize = std::stoul(value);
    } else if (name == "nSlice") {
      _nSlice = std::stoul(value);
    } else if (name == "nCopyThreads") {
      _nCopyThreads = std::stoul(value);
    } else if (name == "cleanInterval") {
      _cleanInterval = std::stoul(value);
    } else if (name == "priorities") {
      _priorities = value;
    } else if (name == "priority") {
      _priority = std::stoul(value);
    } else if (name == "node") {
      _node = value;
    } else if (name == "port") {
      _port = value;
    } else if (name == "database") {
      _database = value;
    } else if (name == "simDisk") {
      _simDisk = (value == "true");
    } else if (name == "simDatabase") {
      _simDatabase = (value == "true");
    } else if (name == "simDcache") {
      _simDcache = (value == "true");
    } else if (name == "simDelay") {
      _simDelay = std::stoul(value);
    } else if (name == "") {
      continue;
    } else {
      std::cout << "Error - RdmConfig could not parse name " << name
                << std::endl;
      return 1;
    }
  }

  return 0;
}
