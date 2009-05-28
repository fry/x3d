#ifndef MESH_HPP_
#define MESH_HPP_

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include <iostream>
#include <vector>

#include "Vector3.hpp"

template<class T> struct Vertex {
  T x, y, z;
};

struct Triangle {
  unsigned short a, b, c;
};

struct MapCoord {
  float u, v;
};

class Mesh {
protected:
  unsigned int m_list;
  float m_radius;
public:
  bool textured;
  std::vector<x3d::Vector3f> vertices;
  std::vector<x3d::Vector3f> normals;
  std::vector<Triangle> polygons;
  std::vector<MapCoord> map_coords;

  Mesh(): m_list(0), textured(false), m_radius(0) {}
  ~Mesh() {
    if (m_list != 0)
      glDeleteLists(m_list, 1);
  }

  void calculate_radius() {
    float radius = 0;
    m_radius = 0;
    for (unsigned int i = 0; i < vertices.size(); i ++) {
      radius = vertices[i].magnitude();
      if (m_radius < radius)
        m_radius = radius;
    }
  }

  float get_radius() {
    return m_radius;
  }

  void create_list() {
    x3d::Vector3f a, b, c;
    x3d::Vector3f normal;
    MapCoord m;

    m_list = glGenLists(1);
    glNewList(m_list, GL_COMPILE);
    glColor3f(1, 1, 1);
    glEnable(GL_LIGHTING);
    for (unsigned int i = 0; i < polygons.size(); i ++) {
      glBegin(GL_TRIANGLES);
        a = vertices[polygons[i].a];
        b = vertices[polygons[i].b];
        c = vertices[polygons[i].c];

        // TODO: check, something appears off with the texture coords
        if (textured) {
          m = map_coords[polygons[i].a];
          glTexCoord2f(m.u, m.v);
        }
        normal = normals[polygons[i].a];
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(a.x, a.y, a.z);
        
        if (textured) {
          m = map_coords[polygons[i].b];
          glTexCoord2f(m.u, m.v);
        }
        normal = normals[polygons[i].b];
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(b.x, b.y, b.z);

        if (textured) {
          m = map_coords[polygons[i].c];
          glTexCoord2f(m.u, m.v);
        }
        normal = normals[polygons[i].c];
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(c.x, c.y, c.z);
      glEnd();
      }
    glEndList();

    calculate_radius();
  }

  void render() {
    if (m_list == 0)
      create_list();

    glPushMatrix();
      glCallList(m_list);
    glPopMatrix();
  }
};

#endif
