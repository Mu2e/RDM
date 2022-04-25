
#include "RDM/inc/RdmThread.hh"

using namespace mu2e;

//***********************************************************

RdmThread::RdmThread(RdmControls &controls, thtype type, size_t copyNo,
                     size_t index) :
    _controls(controls),
    _type(type), _copyNo(copyNo), _index(index) {
  if (type == thtype::discovery) {
    _source = "discovery";
  } else if (type == thtype::declare) {
    _source = "declare";
  } else if (type == thtype::copy) {
    _source = "copy";
  } else if (type == thtype::record) {
    _source = "record";
  } else if (type == thtype::clean) {
    _source = "clean";
  }
  _source += " ";
  _source += std::to_string(copyNo);

  return;
}

/*
int mu2e::RdmThread::write(std::ostream& os) {
  for(auto const& vv: _vcoll) {
    os << vv.csv() << "\n";
  }
  return 0;
}


*/
