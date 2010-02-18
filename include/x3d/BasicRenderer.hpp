#pragma once

#include "x3d/Subsystem.hpp"
#include "x3d/Matrix4.hpp"
#include "x3d/Image.hpp"
#include "x3d/Texture.hpp"
#include "x3d/Color.hpp"

namespace x3d {
namespace Core {

class BasicRenderer: public Subsystem {
public:
  BasicRenderer(): Subsystem("Renderer") {}
  virtual void initialize(int width, int height) = 0;

  virtual void begin() = 0;
  virtual void end() = 0;

  int get_screen_width() { return m_screen_width; }
  int get_screen_height() { return m_screen_height; }

  // Matrix settings
  virtual void set_matrix_projection(const x3d::Matrix4f& mat) = 0;
  virtual void set_matrix_modelview(const x3d::Matrix4f& mat) = 0;

  // Textures
  virtual x3d::Texture* create_texture(x3d::Image* image) = 0;

  virtual void set_clear_color(const x3d::Color& color) = 0;
protected:
  int m_screen_width;
  int m_screen_height;
};

}
}
