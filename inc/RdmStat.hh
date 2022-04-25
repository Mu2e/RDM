#ifndef RDM_RdmStat_hh
#define RDM_RdmStat_hh

//
// holds monitoring info sent from the threads
//

#include <string>

namespace mu2e {

class RdmStat {
 public:
  size_t stat() { return _stat; }

 private:
  size_t _stat;
};

}  // namespace mu2e

#endif
