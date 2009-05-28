#include "x3d/Image.hpp"
#include <SOIL/SOIL.h>

x3d::Image::Image(int channels, int width, int height)
: m_channels(channels), m_width(width), m_height(height) {
  buffer.resize(get_size());
}

x3d::Color x3d::Image::get_color(unsigned int x, unsigned int y) {
  x3d::Color color;
  std::size_t index = x * m_channels + y * m_width * m_channels;
  if (m_channels == 1) { // luminous
    color.set(buffer[index] / 255.0f);
  } else if (m_channels == 2) { // luminous, alpha
    color.set(buffer[index] / 255.0f, buffer[index + 1] / 255.0f);
  } else if (m_channels == 3) { // r, g, b
    color.set(buffer[index] / 255.0f, buffer[index + 1] / 255.0f, buffer[index + 2] / 255.0f);
  } else if (m_channels == 4) { // r, g, b, a
    color.set(buffer[index] / 255.0f, buffer[index + 1] / 255.0f, buffer[index + 2] / 255.0f, buffer[index + 3] / 255.0f);
  }

  return color;
}

int x3d::Image::get_width() {
  return m_width;
}

int x3d::Image::get_height() {
  return m_height;
}

int x3d::Image::get_channels() {
  return m_channels;
}

unsigned int x3d::Image::get_size() {
  return m_channels * m_width * m_height;
}
