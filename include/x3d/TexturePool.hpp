#ifndef X3D_TEXTUREPOOL_HPP_
#define X3D_TEXTUREPOOL_HPP_

#include <boost/ptr_container/ptr_list.hpp>

#include "x3d/ResourcePool.hpp"
#include "x3d/Texture.hpp"
#include "x3d/ImageLoader.hpp"
#include "x3d/Image.hpp"

namespace x3d {
  class TexturePool: public ResourcePool<x3d::Texture> {
  public:
    typedef boost::ptr_list<x3d::ImageLoader> loader_list_type;
    loader_list_type loader;

    resource_type_ptr load(const boost::filesystem::path& file_name);
  };
}

#endif
