
#include "RDM/inc/RdmCache.hh"
#include "RDM/inc/RdmClean.hh"
#include "RDM/inc/RdmConfig.hh"
#include "RDM/inc/RdmControls.hh"
#include "RDM/inc/RdmCopy.hh"
#include "RDM/inc/RdmDeclare.hh"
#include "RDM/inc/RdmDiscovery.hh"
#include "RDM/inc/RdmLogger.hh"
#include "RDM/inc/RdmRecord.hh"
#include "RDM/inc/RdmStat.hh"
#include "RDM/inc/RdmThread.hh"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace mu2e;

namespace {
static void show_usage() {
  std::cerr << "Usage: rawDataMover <options> "
            << "Options:\n"
            << "\t-h,--help    Show this help message\n"
            << "\t-c,--config FILE  start with this configuration" << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  std::string configFile;
  int iarg = 1;
  while (iarg < argc) {
    std::string arg = argv[iarg];
    if ((arg == "-h") || (arg == "--help")) {
      show_usage();
      return 0;
    } else if ((arg == "-c") || (arg == "--config")) {
      if (iarg + 1 < argc) {
        configFile = argv[++iarg];
        std::cout << "loading file " << configFile << std::endl;
      } else {
        std::cerr << "--config option requires file name" << std::endl;
        return 1;
      }
    }
    iarg++;
  }

  size_t nThread{0};
  RdmConfig config;
  if (!configFile.empty()) config.readFile(configFile);
  RdmLogger logger(config);
  RdmStat stat;
  RdmCache cache(config, logger, stat);

  RdmControls controls{config, logger, cache, stat};

  if (config.verbose() > 0) {
    logger.print("startup", "main");
    logger.print(config.toString(), "main");
  }

  // create the objects which go into threads, and keep them in a vector
  std::vector<std::unique_ptr<RdmThread>> thobjs;

  thobjs.push_back(std::make_unique<RdmDiscovery>(controls, 0, nThread++));

  thobjs.push_back(std::make_unique<RdmDeclare>(controls, 0, nThread++));
  for (size_t icopy = 0; icopy < config.nCopyThreads(); icopy++) {
    thobjs.push_back(std::make_unique<RdmCopy>(controls, icopy, nThread++));
  }
  thobjs.push_back(std::make_unique<RdmRecord>(controls, 0, nThread++));
  thobjs.push_back(std::make_unique<RdmClean>(controls, 0, nThread++));

  // create threads of these objects, and keep threads in a vector
  // threads will start running
  std::vector<std::thread> threads;
  for (auto& objptr : thobjs) {
    threads.push_back(std::thread(&RdmThread::execute, objptr.get()));
  }

  while (true) {
    logger.print("loop", "main");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout <<" in main" << std::endl;
  }

  return 0;
}

/*
std::vector<std::unique_ptr<RdmThread>> tv;
tv.push_back( std::make_unique<RdmDiscovery>(config,0,nThread++) );
std::vector<std::thread> thv;
for(auto& p:tv) {
  thv.push_back( std::thread(&(p->execute),*p) );
}
*/

//  thv.push_back( std::thread( &RdmDiscovery::execute,
//                              std::static_pointer_cast<RdmDiscovery>(tv.back()).get()
//                              ) );
// for(auto& p: tv) {
//}
// RdmDiscovery dd(config,0,nThread++);
// std::thread xx(&RdmDiscovery::execute,&dd);
