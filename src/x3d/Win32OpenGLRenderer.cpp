#include "x3d/Win32OpenGLRenderer.hpp"

using namespace x3d::Core;

Win32OpenGLRenderer::Win32OpenGLRenderer(HWND hwnd, int color_bits): m_hwnd(hwnd) {
  m_hdc = GetDC(m_hwnd);
  m_color_bits = color_bits;
}

void Win32OpenGLRenderer::initialize(int width, int height) {
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof(pfd));

  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = m_color_bits;
  pfd.cDepthBits = 24;
  
  pfd.iLayerType = PFD_MAIN_PLANE;
  int format = ChoosePixelFormat(m_hdc, &pfd);
  SetPixelFormat(m_hdc, format, &pfd);

  m_hrc = wglCreateContext(m_hdc);
  
  wglMakeCurrent(m_hdc, m_hrc);

  OpenGLRenderer::initialize(width, height);
}

void Win32OpenGLRenderer::begin() {
  wglMakeCurrent(m_hdc, m_hrc);
  OpenGLRenderer::begin();
}

void Win32OpenGLRenderer::end() {
  OpenGLRenderer::end();
  SwapBuffers(m_hdc);
  wglMakeCurrent(0, 0);
}
