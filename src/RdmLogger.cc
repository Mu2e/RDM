#include "RDM/inc/RdmLogger.hh"

using namespace mu2e;

//***********************************************************

void RdmLogger::print(std::string message, std::string source) {
  std::unique_lock lock(_mutex);

  time_t tt = time(nullptr);
  auto tfp = localtime(&tt);
  std::cout << std::put_time(tfp, "[ %F %T ] ");
  if (!source.empty()) {
    std::cout << "(" << source << ") ";
  }
  std::cout << message << std::endl;

  return;
}
