#pragma once

#include <boost/ptr_container/ptr_list.hpp>

#include "x3d/ResourcePool.hpp"
#include "x3d/Texture.hpp"
#include "x3d/ImageLoader.hpp"
#include "x3d/Image.hpp"
#include "x3d/BasicRenderer.hpp"

namespace x3d {
  class TexturePool: public ResourcePool<x3d::Texture> {
  public:
    TexturePool(x3d::Core::BasicRenderer* renderer);
    typedef boost::ptr_list<x3d::ImageLoader> loader_list_type;
    loader_list_type loader;

    resource_type_ptr load(const boost::filesystem::path& file_name);

  protected:
    x3d::Core::BasicRenderer* m_renderer;
  };
}
