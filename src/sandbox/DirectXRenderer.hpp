#ifndef X3D_DIRECTXRENDERER_HPP_
#define X3D_DIRECTXRENDERER_HPP_

#include "BasicRenderer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class IDirect3D9;
class IDirect3DDevice9;

namespace x3d {
  class DirectXRenderer: public BasicRenderer {
  public:
    DirectXRenderer(HWND hwnd);

    virtual void initialize(int width, int height);

    virtual void begin();
    virtual void end();
  protected:
    
    IDirect3D9* m_d3d;
    IDirect3DDevice9* m_device;

    HWND m_hwnd;
  };
}

#endif
