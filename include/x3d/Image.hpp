#pragma once

#include <vector>
#include <x3d/Color.hpp>

namespace x3d {
  class Image {
  protected:
    int m_width, m_height, m_channels;
    unsigned int m_size;
  public:
    std::vector<unsigned char> buffer;

    Image(int channels, int width, int height);

    x3d::Color get_color(unsigned int x, unsigned int y);
    int get_width();
    int get_height();
    int get_channels();
    unsigned int get_size();
  };
}
