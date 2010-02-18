#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include "x3d/OpenGLRenderer.hpp"
#include "x3d/OpenGLTexture.hpp"

#include <iostream>

namespace {
  void set_matrix(GLenum mode, const x3d::Matrix4<float>& mat) {
    glMatrixMode(mode);
    glLoadMatrixf(mat);
  }

  void set_matrix(GLenum mode, const x3d::Matrix4<double>& mat) {
    glMatrixMode(mode);
    glLoadMatrixd(mat);
  }

  void initialize_texturing() {
    glEnable(GL_TEXTURE_2D);
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  }

  void setup_texture() {
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  }
}

using namespace x3d::Core;

void OpenGLRenderer::begin() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::end() {
}

void OpenGLRenderer::initialize(int width, int height) {
  log().infoStream() << "OpenGL Version: " << glGetString(GL_VERSION);
  log().infoStream() << "Renderer: " << glGetString(GL_RENDERER);
  log().infoStream() << "Vendor: " << glGetString(GL_VENDOR);

  glViewport(0, 0, width, height);
	glClearDepth(1.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glShadeModel(GL_SMOOTH);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_BLEND);

  set_clear_color(x3d::Color::Red);
  initialize_texturing();

  m_screen_width = width;
  m_screen_height = height;
}

void OpenGLRenderer::set_matrix_projection(const x3d::Matrix4f& mat) {
  set_matrix(GL_PROJECTION, mat);
}

void OpenGLRenderer::set_matrix_modelview(const x3d::Matrix4f& mat) {
  set_matrix(GL_MODELVIEW, mat);
}

x3d::Texture* OpenGLRenderer::create_texture(x3d::Image* image) {
  return new x3d::OpenGLTexture(image);
}

void OpenGLRenderer::set_clear_color(const x3d::Color& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
