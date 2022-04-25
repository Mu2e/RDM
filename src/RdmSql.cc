#include "RDM/inc/RdmSql.hh"
#include <iostream>

mu2e::RdmSql::RdmSql() : _conn(nullptr), _verbose(0) {}

// *********************************************************

int mu2e::RdmSql::connect(std::string const& host, std::string const& port,
                          std::string const& database) {
  std::string command;
  command.append(" host=" + host);
  command.append(" port=" + port);
  command.append(" dbname=" + database);

  _conn = PQconnectdb(command.c_str());

  if (_conn) {
    if (PQstatus(_conn) != CONNECTION_OK) {
      _message = PQerrorMessage(_conn);
      return 1;
    }
  } else {
    _message = "database connection failed, pointer not set";
    return 1;
  }
  return 0;
}

// *********************************************************

int mu2e::RdmSql::disconnect() {
  if (_conn) {
    PQfinish(_conn);
    _message = PQerrorMessage(_conn);
  } else {
    _message = "disconnect attempted with no connection";
    return 1;
  }

  return 0;
}

// *********************************************************

int mu2e::RdmSql::execute(const std::string& command, std::string& result) {
  result.clear();
  PGresult* res = PQexec(_conn, command.c_str());

  if (PQresultStatus(res) != PGRES_COMMAND_OK &&
      PQresultStatus(res) != PGRES_TUPLES_OK) {
    _message = PQerrorMessage(_conn);
    PQclear(res);
    return 1;
  }

  size_t nrow = PQntuples(res);
  size_t ncol = PQnfields(res);

  for (size_t r = 0; r < nrow; r++) {
    for (size_t c = 0; c < ncol; c++) {
      result.append(PQgetvalue(res, r, c));
      if (c < ncol - 1) result.append(",");
    }
    result.append("\n");
  }

  return 0;
}
