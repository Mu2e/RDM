#ifndef RDM_RdmPriority_hh
#define RDM_RdmPriority_hh

//
// holds regex for sorting files by priority,
// and can apply that to a file name
//

#include "RDM/inc/RdmConfig.hh"
#include <string>
#include <vector>

namespace mu2e {

class RdmPriority {
 public:
  RdmPriority(RdmConfig& config);

  size_t priority(std::string const& fn) const;

 private:
  typedef std::vector<std::string> StringVec;

  struct pritem {
    std::string test;
    size_t priority;
  };

  StringVec splitString(const std::string& line, char del) const;
  bool is_match(const char* line, const char* pattern) const;
  RdmConfig const& config() const { return _config; }

  RdmConfig _config;
  std::vector<pritem> _items;
};

}  // namespace mu2e

#endif
