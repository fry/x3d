#pragma once

namespace x3d {

class Texture {
protected:
  int m_width, m_height, m_channels;
public:
  virtual ~Texture() {};

  virtual void bind() = 0;

  int get_width();
  int get_height();
  int get_channels();
};

}
