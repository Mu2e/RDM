#ifndef RDM_RdmFile_hh
#define RDM_RdmFile_hh

//
// the record of a file in flight in the RDM
//

#include "RDM/inc/RdmConfig.hh"
#include <ctime>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace mu2e {

class RdmFile {
 public:
  typedef std::unique_ptr<RdmFile> ptr_t;
  typedef std::vector<ptr_t> vec_t;
  typedef std::list<ptr_t> list_t;
  //  typedef std::array<ptr_t, RdmConfig::sliceSize()> slice_t;

  // used when a new file is discovered on disk
  RdmFile(const std::string& dir, const std::string& fn, size_t priority = 0,
          time_t tdeclare = 0, time_t tdcache = 0, time_t ttape = 0,
          time_t terror = 0) :
      _dir(dir),
      _fn(fn), _priority(priority), _tdeclare(tdeclare), _tdcache(tdcache),
      _ttape(ttape), _terror(terror) {}

  // create from a db row
  RdmFile(const std::string& dbrow);

  const std::string& dir() const { return _dir; }
  const std::string& fn() const { return _fn; }
  size_t priority() const { return _priority; }
  time_t tDeclare() const { return _tdeclare; }
  time_t tDcache() const { return _tdcache; }
  time_t tTape() const { return _ttape; }
  time_t tError() const { return _terror; }

  void setPriority(size_t priority) { _priority = priority; }
  void settDeclare(const time_t t) { _tdeclare = t; }
  void settDcache(const time_t t) { _tdcache = t; }
  void settTape(const time_t t) { _ttape = t; }
  void settError(const time_t t) { _terror = t; }

  std::string csv() const {
    std::string cc(200, '\0');
    cc = _dir;
    cc += ",";
    cc += _fn;
    cc += ",";
    cc += std::to_string(_tdeclare);
    cc += ",";
    cc += std::to_string(_tdcache);
    cc += ",";
    cc += std::to_string(_ttape);
    cc += ",";
    cc += std::to_string(_terror);
    return cc;
  }

 private:
  std::string _dir;
  std::string _fn;
  size_t _priority;
  time_t _tdeclare;
  time_t _tdcache;
  time_t _ttape;
  time_t _terror;
};

}  // namespace mu2e

#endif
