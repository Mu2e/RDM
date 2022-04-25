#ifndef RDM_RdmConfig_hh
#define RDM_RdmConfig_hh

//
// holds static and volatile parameters for running the RDM
//

#include <string>

namespace mu2e {

class RdmConfig {
 public:
  RdmConfig() {
    // defaults();
    testMode();
  }

  size_t verbose() const { return _verbose; }
  // number of active slices
  size_t nSlice() const { return _nSlice; }
  // how many files to move together between stages
  size_t sliceSize() const { return _sliceSize; }
  // how many copy threads to allow
  size_t nCopyThreads() const { return _nCopyThreads; }
  // how many seconds between passes at checking and deleting files
  size_t cleanInterval() const { return _cleanInterval; }
  // define how to prioiritize files based on names
  std::string const& priorities() const { return _priorities; }
  // how strongly to favor high priority file (0-10, 10=very)
  size_t priority() const { return _priority; }
  // database connection
  std::string const& node() const { return _node; }
  std::string const& port() const { return _port; }
  std::string const& database() const { return _database; }

  // update content from this file
  int readFile(std::string const& fn);

  std::string toString();

  // tests
  bool simDisk() { return _simDisk; }
  bool simDatabase() { return _simDatabase; }
  bool simDcache() { return _simDcache; }
  int simDelay() { return _simDelay; }
  void testMode();

 private:
  void defaults();

  size_t _verbose;
  size_t _sliceSize;
  size_t _nSlice;
  size_t _nCopyThreads;
  size_t _cleanInterval;
  std::string _priorities;
  size_t _priority;
  std::string _node;
  std::string _port;
  std::string _database;
  bool _simDisk;
  bool _simDatabase;
  bool _simDcache;
  int _simDelay;
};

}  // namespace mu2e

#endif
