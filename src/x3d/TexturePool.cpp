#include "x3d/TexturePool.hpp"

using namespace x3d;

TexturePool::TexturePool(x3d::Core::BasicRenderer* renderer): m_renderer(renderer) {
}

// TODO: Function expects the file to not be loaded
TexturePool::resource_type_ptr
TexturePool::load(const boost::filesystem::path& file_name) {
  for (loader_list_type::iterator it = loader.begin();
       it != loader.end(); it ++) {
    if (it->can_load(file_name)) {
      // Load the image with the appropiate loader
      x3d::Image* image = it->load(file_name);

      std::string name = file_name.string();
      // And create a texture from it and store that
      x3d::Texture* texture = m_renderer->create_texture(image);
      m_resources.insert(name, texture);
      // Clean up and return the texture
      delete image;
      return texture;
    }
  }
  throw std::runtime_error("no loader for " + file_name.string());
}
