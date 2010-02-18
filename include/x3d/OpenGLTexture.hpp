#pragma once

#include "x3d/Texture.hpp"
#include "x3d/Image.hpp"

namespace x3d {

class OpenGLTexture: public x3d::Texture {
protected:
  unsigned int m_id;
public:
  OpenGLTexture(x3d::Image* image);
  virtual ~OpenGLTexture();

  virtual void bind();
};

}
