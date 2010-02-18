#include "x3d/Subsystem.hpp"

#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>

#ifdef __WIN32__
  #include <log4cpp/Win32DebugAppender.hh>
#endif

using namespace x3d::Core;

Subsystem::Subsystem(const std::string& name): m_name(name) {
  log4cpp::Category& cat = log();
  cat.setAdditivity(true);

  log4cpp::Appender* app = new log4cpp::FileAppender("SubsystemLog", m_name + ".log", false);
  log4cpp::Layout* layout = new log4cpp::BasicLayout();
  app->setLayout(layout);

  cat.setAppender(app);
  
#ifdef __WIN32__
  log4cpp::Appender* app_debug = new log4cpp::Win32DebugAppender("Win32DebugLog");
  cat.setAppender(app_debug);
#endif

  cat.setPriority(log4cpp::Priority::DEBUG);

  log().infoStream() << "Initializing subsystem " << m_name;
}

Subsystem::~Subsystem() {
  log4cpp::Category::shutdown();
}

log4cpp::Category& Subsystem::log() {
  return log4cpp::Category::getInstance(m_name);
}
