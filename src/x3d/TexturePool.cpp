#include "x3d/TexturePool.hpp"

x3d::TexturePool::resource_type_ptr
x3d::TexturePool::load(const boost::filesystem::path& file_name) {
  for (loader_list_type::iterator it = loader.begin();
       it != loader.end(); it ++) {
    if (it->can_load(file_name)) {
      // TODO: WHAT
      x3d::Image* image = it->load(file_name);
      std::string name = file_name.string();
      m_resources.insert(name, new x3d::Texture(*image));
      delete image;
      return &m_resources[file_name.string()];
    }
  }
  throw std::runtime_error("No loader for " + file_name.string());
}
