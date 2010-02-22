#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/glew.h>

#include <iostream>
#include <x3d/Vector3.hpp>

#include <x3d/Win32Device.hpp>
#include <x3d/BasicRenderer.hpp>
#include <x3d/TexturePool.hpp>
#include <x3d/ImageLoaderSOIL.hpp>
#include <x3d/Vector2.hpp>
#include <x3d/Color.hpp>

enum ElementsType {
  QUADS, TRIANGLES, TRIANGLE_FAN
};

class OpenGLVBO {
public:
  OpenGLVBO(GLenum type = GL_STATIC_DRAW);

  template <class T> void set_vertex_array(const std::vector<T>& vertices);
  template <class T> void set_normal_array(const std::vector<x3d::Vector3<T>>& normals);
  template <class T> void set_tex_coord_array(const std::vector<x3d::Vector2<T>>& tex_coords);

  template <class T> void add_primitive(ElementsType type, const std::vector<T>& indices);
  void render();
protected:
  GLenum m_type;
  GLuint m_vertex_array;
  GLuint m_normal_array;
  GLuint m_tex_coord_array;
  std::size_t m_vertex_size;

  struct ElementsSet {
    GLenum type;
    GLuint element_type;
    GLuint buffer;
    std::size_t size;
  };

  std::vector<ElementsSet> m_elements;

  template <class  T> GLuint create_buffer(GLenum type, const std::vector<T>& data);
};

OpenGLVBO::OpenGLVBO(GLenum type): m_type(type), m_vertex_array(0), m_normal_array(0), m_tex_coord_array(0) {
}

template <class T> void OpenGLVBO::set_vertex_array(const std::vector<T>& vertices) {
  m_vertex_size = sizeof(T) / 4;
  m_vertex_array = create_buffer(GL_ARRAY_BUFFER, vertices);
}

template <class T> void OpenGLVBO::set_normal_array(const std::vector<x3d::Vector3<T>>& normals) {
  m_normal_array = create_buffer(GL_ARRAY_BUFFER, normals);
}

template <class T> void OpenGLVBO::set_tex_coord_array(const std::vector<x3d::Vector2<T>>& tex_coords) {
  m_tex_coord_array = create_buffer(GL_ARRAY_BUFFER, tex_coords);
}

template <class  T> GLuint OpenGLVBO::create_buffer(GLenum buffer_type, const std::vector<T>& data) {
  assert(data.size() > 0);

  GLuint buffer;
  glGenBuffers(1, &buffer);
  assert(buffer);
  glBindBuffer(buffer_type, buffer);
  glBufferData(buffer_type, data.size() * sizeof(T), &data.front(), m_type);
  return buffer;
}

template <class T> void OpenGLVBO::add_primitive(ElementsType type, const std::vector<T>& indices) {
  ElementsSet set;
  if (type == TRIANGLES) {
    set.type = GL_TRIANGLES;
  } else if (type == QUADS) {
    set.type = GL_QUADS;
  } if (type == TRIANGLE_FAN) {
    set.type = GL_TRIANGLE_FAN;
  } else {
    assert(false);
  }

  std::size_t element_size = sizeof(T);
  assert(element_size > 0 && element_size <= 3);
  if (element_size == 1) {
    set.element_type = GL_UNSIGNED_BYTE;
  } else if (element_size == 2) {
    set.element_type = GL_UNSIGNED_SHORT;
  } else if (element_size == 3) {
    set.element_type = GL_UNSIGNED_INT;
  }

  set.buffer = create_buffer(GL_ELEMENT_ARRAY_BUFFER, indices);
  set.size = indices.size();

  m_elements.push_back(set);
}

void OpenGLVBO::render() {
  if (!m_vertex_array)
    return;

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array);
  glVertexPointer(m_vertex_size, GL_FLOAT, 0, 0);
  
  if (m_normal_array) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, m_normal_array);
    glNormalPointer(GL_FLOAT, 0, 0);
  }

  if (m_tex_coord_array) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, m_tex_coord_array);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
  }

  std::vector<ElementsSet>::iterator it, end = m_elements.end();
  for (it = m_elements.begin(); it != end; ++it) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->buffer);
    glDrawElements(it->type, it->size, it->element_type, 0);
  }

  glDisableClientState(GL_VERTEX_ARRAY);

  if (m_normal_array)
    glDisableClientState(GL_NORMAL_ARRAY);
  if (m_tex_coord_array)
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// MAIN

int main(int argc, char* argv[]) {
  x3d::Core::BasicDevice* device = new x3d::Core::Win32Device(x3d::Core::RENDERER_OPENGL);
  device->initialize(24, 800, 600, "Device/Driver Interface Test");
  device->set_resizeable(true);

  x3d::Core::BasicRenderer* renderer = device->get_renderer();
  renderer->set_clear_color(x3d::Color::Blue);

  x3d::Matrix4f mat; mat.set_ortho(0, 800, 600, 0, -1, 1);
  renderer->set_matrix_projection(mat);

  x3d::TexturePool pool(renderer);
  pool.loader.push_back(new x3d::ImageLoaderSOIL());
  x3d::Texture* texture = pool.load("../frog.bmp");

  OpenGLVBO vbo(GL_STATIC_DRAW);

  std::vector<x3d::Vector2f> vertices;
  vertices.push_back(x3d::Vector2f(0, 0));
  vertices.push_back(x3d::Vector2f(100, 0));
  vertices.push_back(x3d::Vector2f(100, 100));
  vertices.push_back(x3d::Vector2f(0, 100));

  vbo.set_vertex_array(vertices);

  std::vector<x3d::Vector2f> tex_coords;
  tex_coords.push_back(x3d::Vector2f(0, 1));
  tex_coords.push_back(x3d::Vector2f(1, 1));
  tex_coords.push_back(x3d::Vector2f(1, 0));
  tex_coords.push_back(x3d::Vector2f(0, 0));

  vbo.set_tex_coord_array(tex_coords);

  std::vector<unsigned short> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  vbo.add_primitive(TRIANGLES, indices);

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  float x = 0, y = 0;
  while (device->run()) {
    device->get_renderer()->begin();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(x, y, 0);

    texture->bind();

    vbo.render();

    x += (float)rand()/RAND_MAX;
    y += 0.5 * rand()/RAND_MAX;


    device->get_renderer()->end();
  }

  return 0;
}