#ifndef RESOURCEPOOL_HPP_
#define RESOURCEPOOL_HPP_

#include <boost/filesystem/path.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace x3d {
  template <typename T>
  class ResourcePool {
  public:
    typedef T resource_type;
    typedef T* resource_type_ptr;
    typedef boost::ptr_map<std::string, resource_type> resource_map_type;
  protected:
    resource_map_type m_resources;
  public:
    ResourcePool() {}

    bool is_loaded(const std::string& file_name);
    resource_type_ptr get(const boost::filesystem::path& file_name);
    virtual resource_type_ptr load(const boost::filesystem::path& file_name) = 0;
  };
}

template <typename T>
bool x3d::ResourcePool<T>::is_loaded(const std::string& file_name) {
  return m_resources.find(file_name);
}

template <typename T>
typename x3d::ResourcePool<T>::resource_type_ptr
x3d::ResourcePool<T>::get(const boost::filesystem::path& file_name) {
  typename resource_map_type::iterator it = m_resources.find(file_name.string());
  // Load the file if it's not found
  if (it == m_resources.end())
    return load(file_name);

  return it->second;
}

#endif
