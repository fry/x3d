#pragma once

#include <string>
#include <log4cpp/Category.hh>
#undef int64_t

namespace x3d {
namespace Core {

class Subsystem {
public:
  Subsystem(const std::string& name);
  virtual ~Subsystem();

  log4cpp::Category& log();
protected:
  std::string m_name;
};

}
}
