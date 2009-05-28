#ifndef X3D_BASICDEVICE_HPP_
#define X3D_BASICDEVICE_HPP_

#include <boost/scoped_ptr.hpp>
#include <boost/signal.hpp>
#include <string>

namespace x3d {
  class BasicRenderer;
  class BasicDevice {
  public:
    typedef boost::signal<void (
    virtual ~BasicDevice() {};

    virtual bool run() = 0;
    virtual void set_window_name(const std::string& name) = 0;
    virtual void set_resizeable(bool resizeable = true) = 0;
    //virtual void set_resolution(

    BasicRenderer* get_renderer() {
      return m_renderer.get();
    }
  protected:
    boost::scoped_ptr<BasicRenderer> m_renderer;
  };
}

#endif
