#ifndef X3D_BASICDEVICE_HPP_
#define X3D_BASICDEVICE_HPP_

#include <boost/scoped_ptr.hpp>
#include <boost/signal.hpp>
#include <string>

namespace x3d {
  class BasicRenderer;
  enum RendererType { RENDERER_OPENGL, RENDERER_DIRECTX };

  class BasicDevice {
  public:
    virtual ~BasicDevice() {};

    // TODO: add possibility to specify more configurations here
    virtual void initialize(int color_bits, int width, int height, const std::string& name) = 0;
    virtual bool run() = 0;
    virtual void set_title(const std::string& name) = 0;
    virtual void set_resizeable(bool resizeable = true) = 0;
    //virtual void set_size(const x3d::Rectangle<int>& rect) = 0;

    /*virtual BasicRenderer* new_viewport(float left = 0.0f, float top = 0.0f,
                                        float width = 1.0f, float height = 1.0f);*/
    BasicRenderer* get_renderer() {
      return m_renderer.get();
    }
  protected:
    boost::scoped_ptr<BasicRenderer> m_renderer;
  };
}

#endif
