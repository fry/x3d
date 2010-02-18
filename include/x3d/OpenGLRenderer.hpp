#pragma once

#include "BasicRenderer.hpp"

namespace x3d {
namespace Core {

class OpenGLRenderer: public BasicRenderer {
public:
  virtual void initialize(int width, int height) = 0;

  virtual void begin();
  virtual void end();

  // Matrix settings
  virtual void set_matrix_projection(const x3d::Matrix4f& mat);
  virtual void set_matrix_modelview(const x3d::Matrix4f& mat);

  // Textures
  virtual x3d::Texture* create_texture(x3d::Image* image);

  virtual void set_clear_color(const x3d::Color& color);
};

}
}
