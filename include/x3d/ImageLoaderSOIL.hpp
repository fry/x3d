#pragma once

#include "x3d/ImageLoader.hpp"

namespace x3d {
  class ImageLoaderSOIL: public ImageLoader {
  public:
    bool can_load(const boost::filesystem::path& file_name);
    x3d::Image* load(const boost::filesystem::path& file_name);
  };
}
