#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "OpenGLRenderer.hpp"

namespace x3d {
namespace Core {

class Win32OpenGLRenderer: public OpenGLRenderer {
public:
  Win32OpenGLRenderer(HWND hwnd, int color_bits);
  virtual void initialize(int width, int height);
  virtual void begin();
  virtual void end();
protected:
  HWND m_hwnd;
  HDC m_hdc;
  HGLRC m_hrc;

  int m_color_bits;
};

}
}
