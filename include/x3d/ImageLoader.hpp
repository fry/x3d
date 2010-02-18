#pragma once

#include <boost/filesystem/path.hpp>
#include "x3d/Image.hpp"

namespace x3d {
  class ImageLoader {
  public:
    // TODO: when implementing a VFS, have it being passed in the constructor
    // ModelLoader(x3d::file_system* file_system);
    // Returns whether this loader can load a file with the specified name.
    virtual bool can_load(const boost::filesystem::path& file_name) = 0;
    virtual x3d::Image* load(const boost::filesystem::path& file_name) = 0;

    virtual ~ImageLoader() {}
  };
}
