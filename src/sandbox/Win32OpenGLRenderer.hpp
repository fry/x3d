#ifndef X3D_WIN32OPENGLRENDERER_HPP_
#define X3D_WIN32OPENGLRENDERER_HPP_

#include "OpenGLRenderer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace x3d {
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

#endif
