#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <map>
#include <string>
#include <boost/filesystem/path.hpp>

namespace x3d {
  class Image;
  class Texture {
  protected:
    unsigned int m_id;
    int m_width, m_height, m_channels;
  public:
    Texture() {}
    Texture(x3d::Image& image);
    ~Texture();

    void bind();

    int get_width();
    int get_height();
    int get_channels();
  };
}

#endif
