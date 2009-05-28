#include "OpenGLRenderer.hpp"

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include <iostream>

void x3d::OpenGLRenderer::begin() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void x3d::OpenGLRenderer::end() {
}

void x3d::OpenGLRenderer::initialize(int width, int height) {
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

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

  m_screen_width = width;
  m_screen_height = height;
}