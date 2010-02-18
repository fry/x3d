#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/glew.h>
#include <gl/glu.h>


#include <iostream>
#include <x3d/Vector3.hpp>

#include <x3d/Win32Device.hpp>
#include <x3d/BasicRenderer.hpp>
#include <x3d/TexturePool.hpp>
#include <x3d/ImageLoaderSOIL.hpp>
#include <x3d/Vector2.hpp>
#include <x3d/Color.hpp>

struct my_vertex_type {
  my_vertex_type(const x3d::Vector2f& _pos, float _u, float _v): pos(_pos), u(_u), v(_v) {}
  x3d::Vector2f pos;
  float u, v;
};

bool init_glew() {
  GLenum ret = glewInit();

  if (GLEW_OK != ret) {
    std::cout << "Error: " << glewGetErrorString(ret) << std::endl;
    return false;
  }

  if (!GLEW_ARB_vertex_buffer_object) {
    std::cout << "no VBOs" << std::endl;
    return false;
  }

  return true;
}

int main(int argc, char* argv[]) {
  x3d::Core::BasicDevice* device = new x3d::Core::Win32Device(x3d::Core::RENDERER_OPENGL);
  device->initialize(24, 800, 600, "Device/Driver Interface Test");
  device->set_resizeable(true);

  if (!init_glew()) {
    std::cin.get();
    return -1;
  }

  x3d::Core::BasicRenderer* renderer = device->get_renderer();
  renderer->set_clear_color(x3d::Color::Blue);

  x3d::Matrix4f mat; mat.set_ortho(0, 800, 600, 0, -1, 1);
  renderer->set_matrix_projection(mat);

  x3d::TexturePool pool(renderer);
  pool.loader.push_back(new x3d::ImageLoaderSOIL());
  x3d::Texture* texture = pool.load("../frog.png");

  std::vector<my_vertex_type> vertices;
  vertices.push_back(my_vertex_type(x3d::Vector2f(0, 0), 0, 1));
  vertices.push_back(my_vertex_type(x3d::Vector2f(100, 0), 1, 1));
  vertices.push_back(my_vertex_type(x3d::Vector2f(100, 100), 1, 0));
  vertices.push_back(my_vertex_type(x3d::Vector2f(0, 100), 0, 0));

  GLuint buffer;
  glGenBuffersARB(1, &buffer);
  glBindBufferARB(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(my_vertex_type), &vertices.front(), GL_STATIC_DRAW);

  std::vector<unsigned short> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  GLuint buffer_indices;
  glGenBuffers(1, &buffer_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices.front(), GL_STATIC_DRAW);

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  float x = 0, y = 0;
  while (device->run()) {
    device->get_renderer()->begin();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glTranslatef(x, y, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);

    glVertexPointer(2, GL_FLOAT, sizeof(my_vertex_type), 0);
    //glColorPointer(4, GL_FLOAT, sizeof(my_vertex_type), (void*)(sizeof(x3d::Vector2f) * sizeof(float)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(my_vertex_type), (void*)(sizeof(x3d::Vector2f) * sizeof(float)));
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    /*glBegin(GL_QUADS);
      glTexCoord2f(0, 1);
      glVertex2f(0, 0);
      glTexCoord2f(1, 1);
      glVertex2f(1, 0);
      glTexCoord2f(1, 0);
      glVertex2f(1, 1);
      glTexCoord2f(0, 0);
      glVertex2f(0, 1);
    glEnd();*/

    x += (float)rand()/RAND_MAX;
    y += 0.5 * rand()/RAND_MAX;


    device->get_renderer()->end();
  }

  return 0;
}