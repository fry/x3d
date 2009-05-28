#ifndef X3D_MODELOBJ_HPP_
#define X3D_MODELOBJ_HPP_

#include "x3d/ModelLoader.hpp"
#include "x3d/TexturePool.hpp"

namespace x3d {
  class ModelLoaderOBJ: public ModelLoader {
    x3d::TexturePool& m_texture_pool;
  public:
    ModelLoaderOBJ(x3d::TexturePool& texture_pool): m_texture_pool(texture_pool) {};
    bool can_load(const boost::filesystem::path& file_name);
    virtual x3d::Model<float>* load(const boost::filesystem::path& file_name);
  };
}

#endif