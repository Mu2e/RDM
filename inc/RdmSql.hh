#ifndef RDM_RdmSql_hh
#define RDM_RdmSql_hh

#include <libpq-fe.h>
#include <string>

namespace mu2e {

class RdmSql {
 public:
  RdmSql();

  int connect(std::string const& host, std::string const& port,
              std::string const& database);
  int disconnect();
  int execute(std::string const& command, std::string& result);

  std::string const& message() const { return _message; }

  void setVerbose(size_t v) { _verbose = v; }

 private:
  PGconn* _conn;
  size_t _verbose;
  std::string _message;
};

}  // namespace mu2e
#endif
