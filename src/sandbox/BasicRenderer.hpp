#ifndef X3D_BASICRENDERER_HPP_H
#define X3D_BASICRENDERER_HPP_H

namespace x3d {
  class BasicRenderer {
  public:
    virtual void initialize(int width, int height) = 0;

    virtual void begin() = 0;
    virtual void end() = 0;

    int get_screen_width() { return m_screen_width; }
    int get_screen_height() { return m_screen_height; }
  protected:
    int m_screen_width;
    int m_screen_height;
  };
}

#endif
