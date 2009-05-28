#include "Win32Device.hpp"
#include "Win32OpenGLRenderer.hpp"

#include <string>
#include <map>
#include <iostream>

// A map mapping a window handle to its Win32Device, this needs to be global so WndProc can
// know about it
std::map<HWND, x3d::Win32Device*> g_devices;

LRESULT CALLBACK WndProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam) {
  x3d::Win32Device* dev;// = g_devices[hwnd];
  switch(msg) {
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_SIZE:
      RECT rect;
      GetClientRect(hwnd, &rect);
      std::cout << "resized " << wparam << ", " << SIZE_RESTORED << std::endl;
      std::cout << rect.right << "x" << rect.bottom << std::endl;
      break;
    default:
      return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  return 0;
}

x3d::Win32Device::Win32Device(int color_bits, int width, int height, const std::string& name,
                              bool resizeable) {
  static const char* class_name = "Win32Window";
  WNDCLASSEX wc;

  HINSTANCE hinstance = GetModuleHandle(0);

  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hinstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = class_name;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wc)) {
    throw std::runtime_error("Window creation failed");
  }

  // Set window style
  DWORD style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  // TODO:
  if (resizeable)
    style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
  /*if (fullscreen)
    style = WS_POPUP;*/

  RECT client_size;
  client_size.left = 0;
  client_size.top = 0;
  client_size.right = width;
  client_size.bottom = height;

  // Determine window size from client area size;
  AdjustWindowRect(&client_size, style, false);

  int window_width = client_size.right - client_size.left;
  int window_height = client_size.bottom - client_size.top;

  m_hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    class_name,
    name.c_str(),
    style,
    CW_USEDEFAULT, CW_USEDEFAULT,
    window_width, window_height,
    0, 0,
    hinstance, 0);

  if (m_hwnd == 0) {
    // TODO: create a more specialised exception class
    throw std::runtime_error("Window creation failed");
  }

  g_devices[m_hwnd] = this;

  ShowWindow(m_hwnd, SW_SHOW);
  UpdateWindow(m_hwnd);

  // Load renderer
  m_renderer.reset(new x3d::Win32OpenGLRenderer(m_hwnd, color_bits, width, height));
}

bool x3d::Win32Device::run() {
  bool quit = false;
  MSG msg;
  while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (msg.message == WM_QUIT)
      quit = true;
  }

  return !quit;
}

void x3d::Win32Device::set_window_name(const std::string& name) {
  SetWindowTextA(m_hwnd, name.c_str());
}

void x3d::Win32Device::set_resizeable(bool resizeable) {
  DWORD style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  if (resizeable) {
    style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
  }

  // Retrieve current position
  RECT window_size;
  GetWindowRect(m_hwnd, &window_size);

  // Apply new style
  SetWindowLong(m_hwnd, GWL_STYLE, style);

  RECT client_size;
  client_size.left = 0;
  client_size.top = 0;
  client_size.right = get_renderer()->get_screen_width();
  client_size.bottom = get_renderer()->get_screen_height();

  // Determine window size from client area size;
  AdjustWindowRect(&client_size, style, false);

  int window_width = client_size.right - client_size.left;
  int window_height = client_size.bottom - client_size.top;
  
  SetWindowPos(m_hwnd, HWND_TOP, window_size.left, window_size.top, window_width, window_height,
    SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
}
