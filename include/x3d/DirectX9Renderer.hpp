#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "BasicRenderer.hpp"

struct IDirect3D9;
struct IDirect3DDevice9;

namespace x3d {
namespace Core {

class DirectX9Renderer: public BasicRenderer {
public:
  DirectX9Renderer(HWND hwnd);

  virtual void initialize(int width, int height);

  virtual void begin();
  virtual void end();

  // Matrix settings
  virtual void set_matrix_projection(const x3d::Matrix4f& mat);
  virtual void set_matrix_modelview(const x3d::Matrix4f& mat);

  // Textures
  virtual x3d::Texture* create_texture(x3d::Image* image);

  virtual void set_clear_color(const x3d::Color& color);
protected:
  
  IDirect3D9* m_d3d;
  IDirect3DDevice9* m_device;

  HWND m_hwnd;
};

}
}
