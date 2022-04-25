#ifndef RDM_RdmControls_hh
#define RDM_RdmControls_hh

//
// a struct to hold references that need to be passed around
//

#include "RDM/inc/RdmCache.hh"
#include "RDM/inc/RdmConfig.hh"
#include "RDM/inc/RdmLogger.hh"
#include "RDM/inc/RdmStat.hh"

namespace mu2e {

struct RdmControls {
  RdmConfig& _config;
  RdmLogger& _logger;
  RdmCache& _cache;
  RdmStat& _stat;
};

}  // namespace mu2e

#endif
