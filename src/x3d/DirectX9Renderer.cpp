#include "x3d/DirectX9Renderer.hpp"
#include <d3dx9.h>
#include <stdexcept>

using namespace x3d::Core;

DirectX9Renderer::DirectX9Renderer(HWND hwnd): m_hwnd(hwnd) {
}

void DirectX9Renderer::initialize(int width, int height) {
  m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

  HRESULT res;
  D3DDISPLAYMODE d3ddm;

  m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

  D3DPRESENT_PARAMETERS pp;
  ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS));
  pp.BackBufferFormat = d3ddm.Format;
  pp.EnableAutoDepthStencil = true;
  pp.AutoDepthStencilFormat = D3DFMT_D16;
  pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  pp.Windowed = true;

  res = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &pp, &m_device);

  if (FAILED(res)) {
    // TODO: specialize exception
    throw std::runtime_error("Failed creating device");
  }

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX proj;
  D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45.0f), 1.0f, 1.0f, 100.0f);
  m_device->SetTransform(D3DTS_PROJECTION, &proj);
}

void DirectX9Renderer::begin() {
  m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.2, 0, 0.2, 0), 1.0f, 0);
  m_device->BeginScene();
}

void DirectX9Renderer::end() {
  m_device->EndScene();
  m_device->Present(NULL, NULL, NULL, NULL);
}

void DirectX9Renderer::set_matrix_projection(const x3d::Matrix4f& mat) {
  // TODO: NYI
}

void DirectX9Renderer::set_matrix_modelview(const x3d::Matrix4f& mat) {
  // TODO: NYI
}

x3d::Texture* DirectX9Renderer::create_texture(x3d::Image* image) {
  // TODO: NYI
  return NULL;
}

void DirectX9Renderer::set_clear_color(const x3d::Color& color) {
  // TODO: NYI
}
