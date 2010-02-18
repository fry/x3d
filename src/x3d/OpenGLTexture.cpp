#include "x3d/OpenGLTexture.hpp"

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include <SOIL/SOIL.h>

using namespace x3d;
// Load texture from image
OpenGLTexture::OpenGLTexture(x3d::Image* image): m_id(0) {
  m_id = SOIL_create_OGL_texture(
    &image->buffer.front(), image->get_width(), image->get_height(), image->get_channels(),
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
	);

  if (m_id == 0) {
    std::string error_str = "error loading texture from image: ";
    error_str += SOIL_last_result();
    throw std::runtime_error(error_str);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  m_width = image->get_width();
  m_height = image->get_height();
  m_channels = image->get_channels();
}

OpenGLTexture::~OpenGLTexture() {
  glDeleteTextures(1, &m_id);
}

void OpenGLTexture::bind() {
  glBindTexture(GL_TEXTURE_2D, m_id);
}
