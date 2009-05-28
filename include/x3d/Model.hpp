#ifndef X3D_MODEL_HPP_
#define X3D_MODEL_HPP_

#include <boost/filesystem/path.hpp>
#include <boost/shared_array.hpp>

#include "x3d/Vector3.hpp"
#include "x3d/Vector2.hpp"
#include "Material.hpp"

namespace x3d {
  template <typename T> class Model {
  public:
    struct vertex_type {
      x3d::Vector3<T> position;

      vertex_type() {}
      vertex_type(const x3d::Vector3<T>& pos): position(pos) {}
    };

    struct face_type {
      unsigned int vertex_index[3];
      x3d::Vector3<T> normal[3];
      x3d::Vector2<T> texcoord[3];
    };

    struct mesh_type {
      int material_index;
      std::vector<unsigned int> face_indices;

      mesh_type(): material_index(-1) {}
    };

    typedef std::vector<vertex_type> vertex_list_type;
    typedef std::vector<face_type> face_list_type;
    typedef std::vector<x3d::Material> material_list_type;
    typedef std::vector<mesh_type> mesh_list_type;

    Model();
    void render();

    //virtual void load(const boost::filesystem::path& file_name) = 0;

    std::size_t get_vertex_count() { return vertices.size(); }
    std::size_t get_face_count() { return faces.size(); }
  
    vertex_list_type vertices;
    face_list_type faces;
    material_list_type materials;
    mesh_list_type meshes;

  protected:
    // display list
    unsigned int m_list;
  };
}

template <typename T>
x3d::Model<T>::Model() {
  m_list = 0;
}

template <typename T>
void x3d::Model<T>::render() {
  if (m_list == 0) {
    m_list = glGenLists(1);
    glNewList(m_list, GL_COMPILE);
    // render the meshes
    unsigned int index;
    x3d::Material* material;
    mesh_list_type::iterator it, end;
    end = meshes.end();

    for (it = meshes.begin(); it != end; it ++) {
      if (it->material_index == -1) {
        glDisable(GL_TEXTURE_2D);
      } else {
        material = &materials[it->material_index];
        if (material->texture != 0) {
          glEnable(GL_TEXTURE_2D);
        } else {
          glDisable(GL_TEXTURE_2D);
        }
        material->apply();
      }
      glBegin(GL_TRIANGLES);
      std::vector<unsigned int>::iterator face_end = it->face_indices.end();
      for (std::vector<unsigned int>::iterator face_it = it->face_indices.begin();
           face_it != face_end;
           face_it ++) {
        for (int v = 0; v < 3; v ++) {
          index = *face_it;
          glTexCoord2fv(faces[index].texcoord[v]);
          glNormal3fv(faces[index].normal[v]);
          glVertex3fv(vertices[faces[index].vertex_index[v]].position);
        }
      }
      glEnd();
    }
    glEndList();
  }
  if (m_list != 0)
    glCallList(m_list);
}

#endif