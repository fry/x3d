#include "Win32OpenGLRenderer.hpp"

x3d::Win32OpenGLRenderer::Win32OpenGLRenderer(HWND hwnd, int color_bits, int width, int height): m_hwnd(hwnd) {
  m_hdc = GetDC(m_hwnd);

  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof(pfd));

  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = color_bits;
  pfd.cDepthBits = 24;
  
  pfd.iLayerType = PFD_MAIN_PLANE;
  int format = ChoosePixelFormat(m_hdc, &pfd);
  SetPixelFormat(m_hdc, format, &pfd);

  m_hrc = wglCreateContext(m_hdc);
  
  wglMakeCurrent(m_hdc, m_hrc);
  initialize(width, height);
}

void x3d::Win32OpenGLRenderer::begin() {
  wglMakeCurrent(m_hdc, m_hrc);
  x3d::OpenGLRenderer::begin();
}

void x3d::Win32OpenGLRenderer::end() {
  x3d::OpenGLRenderer::end();
  SwapBuffers(m_hdc);
  wglMakeCurrent(0, 0);
}