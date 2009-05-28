#include "x3d/ModelMS3D.hpp"

#include <boost/filesystem/fstream.hpp>
#include <iostream>
/*
// helper functions for reading binary data from a file stream
namespace {
  template <typename T>
  T read(boost::filesystem::ifstream& stream) {
    T value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
  }

  std::string read_string(boost::filesystem::ifstream& stream, int length) {
    char* buffer = new char[length];
    stream.read(buffer, length);
    std::string str(buffer, length);
    delete buffer;
    return str;
  }

  std::string read_null_string(boost::filesystem::ifstream& stream) {
    std::string str;
    char chr;
    do {
      chr = stream.get();
      str += chr;
    } while (chr != '\0');
    return str;
  }
}

void x3d::ModelMS3D::load(const boost::filesystem::path& file_name) {
  if (!boost::filesystem::exists(file_name)) {
    throw std::runtime_error("File not found");
  }

  boost::filesystem::ifstream stream(file_name, std::ios::in | std::ios::binary);

  if (!stream.good()) {
    throw std::runtime_error("Cannot read file");
  }

  // read header
  std::string id = read_string(stream, 10);
  int version = read<int>(stream);
  // throw an exception if the version isn't 4
  if (version != 4)
    throw std::runtime_error("MS3D version mismatch");

  // read vertices
  unsigned short vertex_count = read<unsigned short>(stream);
  std::cout << "vertex count: " << vertex_count << std::endl;
  m_vertices.reserve(vertex_count);
  vertex_type vertex;
  for (int i = 0; i < vertex_count; i ++) {
    if (stream.eof())
      throw std::runtime_error("end of file reached while reading vertices");
    stream.get();      // flags
    vertex = read<vertex_type>(stream); // read in the vertex
    stream.get();      // bone id
    stream.get();      // reference count
    m_vertices.push_back(vertex);
  }

  // read faces
  unsigned short face_count = read<unsigned short>(stream);
  std::cout << "face count: " << face_count << std::endl;
  m_faces.reserve(face_count);
  face_type face;
  x3d::Vector3f normal;
  for (int i = 0; i < face_count; i ++) {
    if (stream.eof())
      throw std::runtime_error("end of file reached while reading faces");
    read<unsigned short>(stream);       // flags
    face.vertex_index[0] = read<unsigned short>(stream); // index 1
    face.vertex_index[1] = read<unsigned short>(stream); // index 2
    face.vertex_index[2] = read<unsigned short>(stream); // index 3

    // read in the three normals
    for (int j = 0; j < 3; j ++) {
      normal = read<x3d::Vector3f>(stream);
      face.normal[j] = normal;
    }
    // read texcoords
    // read u
    for (int j = 0; j < 3; j ++) {
      face.texcoord[j].x = read<float>(stream);
    }
    // read v, they are reversed because milkshape uses a top-left coordinate system
    for (int j = 0; j < 3; j ++) {
      float t = read<float>(stream);
      face.texcoord[j].y = 1.0f - std::abs(t);
      //std::cout << "t:" << t << "/" << face.texcoord[j].y << std::endl;
    }
    stream.get();       // smoothing group
    stream.get();       // group index

    m_faces.push_back(face);
  }

  // read mesh groups
  unsigned short mesh_count = read<unsigned short>(stream);
  unsigned short mesh_face_count;
  mesh_type mesh;
  std::cout << "mesh count: " << mesh_count << std::endl;
  m_meshes.reserve(mesh_count);
  for (int i = 0; i < mesh_count; i ++) {
    stream.get();
    std::cout << "  name: " << read_string(stream, 32) << std::endl; // name
    unsigned short face_count = read<unsigned short>(stream);
    mesh.face_indices.reserve(face_count);
    for (int j = 0; j < face_count; j ++) {
      mesh.face_indices.push_back(read<unsigned short>(stream));
    }
    mesh.material_index = read<char>(stream);
    std::cout << "  material index: " << mesh.material_index << std::endl;

    m_meshes.push_back(mesh);
  }

  // read materials
  unsigned short material_count = read<unsigned short>(stream);
  std::cout << "material count: " << material_count << std::endl;
  x3d::Material material;
  for (int i = 0; i < material_count; i ++) {
    std::cout << "  name: " << read_string(stream, 32) << std::endl;
    material.ambient = read<x3d::Color>(stream);
    material.diffuse = read<x3d::Color>(stream);
    material.specular = read<x3d::Color>(stream);
    material.emission = read<x3d::Color>(stream);
    material.shininess = read<float>(stream);
    read<float>(stream); // transparency
    stream.get(); // mode
    std::string texture_name = read_string(stream, 128).c_str(); // texture name
    if (!texture_name.empty()) {
      // load texture
      //texture_name = "dummy.jpg";
      std::cout << "  texture name: " << texture_name << std::endl;
      material.texture.reset(new x3d::Texture(file_name.branch_path() / texture_name));
      std::cout << "    width: " << material.texture->get_width() << std::endl;
      std::cout << "    height: " << material.texture->get_height() << std::endl;
    }
    
    read_string(stream, 128); // alpha map name

    m_materials.push_back(material);
  }

  // sorting the meshes so the transparent ones are drawn last, pretty hackish (read: dumb),
  // but enugh for now
  for(int i = 0; i < m_meshes.size(); i ++) {
    if (m_meshes[i].material_index < 0)
      continue;

    if (m_materials[m_meshes[i].material_index].transparent()) {
      m_meshes.insert(m_meshes.end(), m_meshes[i]);
      m_meshes.erase(m_meshes.begin() + i);
    }
  }
  
}

// loads a model from a file and returns a pointer to it
x3d::ModelMS3D* x3d::load_ms3d(const boost::filesystem::path& file_name) {
  x3d::ModelMS3D* model = new x3d::ModelMS3D();
  model->load(file_name);
  return model;
}
*/