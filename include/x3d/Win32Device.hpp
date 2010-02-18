#ifndef X3D_WIN32DEVICE_HPP_
#define X3D_WIN32DEVICE_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "BasicDevice.hpp"
#include "BasicRenderer.hpp"

#include <string>

namespace x3d {
namespace Core {

class Win32Device: public BasicDevice {
public:
  Win32Device(RendererType renderer);
  virtual void initialize(int color_bits, int width, int height, const std::string& name);
  virtual ~Win32Device() {};

  virtual bool run();
  virtual void set_title(const std::string& name);
  virtual void set_resizeable(bool resizeable = true);
protected:
  HWND m_hwnd;
  RendererType m_renderer_type;
};

}
}

#endif