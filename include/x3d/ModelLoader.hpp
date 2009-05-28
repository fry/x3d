#ifndef X3D_MODELLOADER_HPP_
#define X3D_MODELLOADER_HPP_

#include <boost/filesystem/path.hpp>

#include "Model.hpp"
namespace x3d {
  class ModelLoader {
  public:
    // TODO: when implementing a VFS, have it being passed in the constructor
    // ModelLoader(x3d::file_system* file_system);
    // Returns whether this loader can load a file with the specified name.
    virtual bool can_load(const boost::filesystem::path& file_name) = 0;
    virtual x3d::Model<float>* load(const boost::filesystem::path& file_name) = 0;

    virtual ~ModelLoader() {}
  };
}

#endif