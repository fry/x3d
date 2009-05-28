#ifndef X3D_MATERIAL_HPP_
#define X3D_MATERIAL_HPP_

#include "Color.hpp"
#include "x3d/Texture.hpp"

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include <iostream>

namespace x3d {
  class Material {
  public:
    x3d::Color ambient;
    x3d::Color diffuse;
    x3d::Color specular;
    x3d::Color emission;
    float shininess;
    boost::shared_ptr<x3d::Texture> texture;

    Material() {
      ambient.set(0.2, 0.2, 0.2, 1.0);
      diffuse.set(0.8, 0.8, 0.8, 1.0);
      specular.set(1.0, 1.0, 1.0, 1.0);
      emission.set(0.0, 0.0, 0.0, 1.0);
      shininess = 0;
    }

    void apply() {
      //glEnable(GL_BLEND);
      glColor3f(1, 1, 1);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glMaterialfv(GL_FRONT, GL_EMISSION, emission);
      glMaterialfv(GL_FRONT, GL_SHININESS, reinterpret_cast<float*>(&shininess));

      if (texture != 0) {
        texture->bind();
      }
    }

    bool transparent() {
      if (ambient.a < 1.0 || diffuse.a < 1.0)
        return true;
      return false;
    }
  };
}

#endif