#ifndef X3D_OPENGLRENDERER_HPP_
#define X3D_OPENGLRENDERER_HPP_

#include "BasicRenderer.hpp"

namespace x3d {
  class OpenGLRenderer: public BasicRenderer {
  public:
    virtual void begin();
    virtual void end();
  protected:
    void initialize(int width, int height);
  };
}

#endif
