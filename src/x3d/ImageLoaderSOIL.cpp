#include "x3d/ImageLoaderSOIL.hpp"
#include "x3d/Image.hpp"
#include <iostream>
#include <SOIL/SOIL.h>

bool x3d::ImageLoaderSOIL::can_load(const boost::filesystem::path& file_name) {
  return true; // TODO: replace with what SOIL can load
}

x3d::Image* x3d::ImageLoaderSOIL::load(const boost::filesystem::path& file_name) {
  x3d::Image* image;

  int width, height, channels;
  unsigned char* buffer;
  buffer = SOIL_load_image(
    file_name.string().c_str(),
    &width, &height, &channels,
    SOIL_LOAD_AUTO
  );

  if (!buffer) {
    std::string error_str = "failed to load image: ";
    error_str += SOIL_last_result();
    throw new std::runtime_error(error_str);
  }

  image = new x3d::Image(channels, width, height);
  // copy pixel data into class local buffer
  image->buffer.assign(buffer, buffer + image->get_size());
  delete[] buffer;

  return image;
}
