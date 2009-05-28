#ifndef X3D_MODEL_POOL_HPP_
#define X3D_MODEL_POOL_HPP_

#include "x3d/ResourcePool.hpp"
#include "x3d/ModelLoader.hpp"
#include "x3d/TexturePool.hpp"

namespace x3d {
  template <typename T = float>
  class ModelPool: public ResourcePool<x3d::Model<T> > {
  public:

    typedef boost::ptr_list<x3d::ModelLoader> loader_list_type;
    loader_list_type loader;

    resource_type* load(const boost::filesystem::path& file_name);
  };
}

template <typename T>
typename x3d::ModelPool<T>::resource_type_ptr
x3d::ModelPool<T>::load(const boost::filesystem::path& file_name) {
  for (loader_list_type::iterator it = loader.begin();
       it != loader.end(); it ++) {
    if (it->can_load(file_name)) {
      // TODO: WHAT
      m_resources.insert(std::string(file_name.string()), it->load(file_name));
      return &m_resources[file_name.string()];
      //return m_resources[file_name] = it->load(file_name);
    }
  }
  throw std::runtime_error("No loader for " + file_name.string());
}

#endif