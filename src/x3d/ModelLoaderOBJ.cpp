#include "x3d/ModelLoaderOBJ.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

namespace {
  template <typename T>
  T read(boost::filesystem::ifstream& stream) {
    T v;
    stream >> v;
    return v;
  }

  std::string read_line(boost::filesystem::ifstream& stream) {
    std::string line;
    std::getline(stream, line);
    return line;
  }

  // from http://www.oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
  // modified to include empty tokens and return token count
  int split(const std::string& str, 
            std::vector<std::string>& tokens,
            const std::string& delimiters = " ") {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = 0;//str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos  = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      if (pos == std::string::npos)
        break;
      lastPos = pos + 1;//str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }

    return tokens.size();
  }
}

bool x3d::ModelLoaderOBJ::can_load(const boost::filesystem::path& file_name) {
  return file_name.string().substr(file_name.string().find_last_of(".") + 1) == "obj";
}

x3d::Model<float>* x3d::ModelLoaderOBJ::load(const boost::filesystem::path& file_name) {
  if (!boost::filesystem::exists(file_name)) {
    throw std::runtime_error("File not found");
  }

  boost::filesystem::ifstream stream(file_name);

  if (!stream.good()) {
    throw std::runtime_error("Cannot read file");
  }

  
  x3d::Model<float>* model = new x3d::Model<float>();
  // read file and check how many entries are needed first
  int texcoord_count = 0; int vertex_count = 0; int normal_count = 0; int face_count = 0;
  while (stream.good()) {
    std::string command;
    command = read<std::string>(stream);
    if (command == "v")
      vertex_count ++;
    else if (command == "vt")
      texcoord_count ++;
    else if (command == "vn")
      normal_count ++;
    else if (command == "f")
      face_count ++;

    read_line(stream);
  }

  // temporary storage for texcoords and normals so they can be converted to our internal
  // format when the faces are read
  // also reserve the previously calculated amount of storage
  std::vector<x3d::Vector2f> texcoords;
  std::vector<x3d::Vector3f> normals; 
  std::vector<unsigned int> faces; 
  std::map<std::string, int> materials; // associated material name with material index

  model->vertices.reserve(vertex_count);
  texcoords.reserve(texcoord_count);
  normals.reserve(normal_count);
  faces.reserve(face_count);

  // now reset the pointer and read the actual data
  stream.seekg(0); stream.clear();
  // TODO: apparently the previous code doesn't properly reset the get pointer
  stream.close(); stream.open(file_name);
  while (stream.good()) {
    std::string command;
    command = read<std::string>(stream);
    if (command == "v") { // read a vertex
      x3d::Model<float>::vertex_type vertex;
      vertex.position.x = read<float>(stream);
      vertex.position.y = read<float>(stream);
      vertex.position.z = read<float>(stream);
      //std::cout << "vertex: " << vertex.position.serialize() << std::endl;
      model->vertices.push_back(vertex);
    } else if (command == "vt") { // read a texture coordinate
      x3d::Vector2f texcoord;
      texcoord.x = read<float>(stream);
      texcoord.y = read<float>(stream);
      //std::cout << "texcoord: " << texcoord.serialize() << std::endl;
      texcoords.push_back(texcoord);
    } else if (command == "vn") { // read a normal
      x3d::Vector3f normal;
      normal.x = read<float>(stream);
      normal.y = read<float>(stream);
      normal.z = read<float>(stream);
      //std::cout << "normal: " << normal.serialize() << std::endl;
      normals.push_back(normal);
    } else if (command == "g") { // read a mesh group
      std::cout << "new group: " << read<std::string>(stream) << std::endl;
      model->meshes.push_back(x3d::Model<float>::mesh_type());
    } else if (command == "usemtl") { // set group's material
      // add a default mesh if there's none present
      if (model->meshes.empty())
        model->meshes.push_back(x3d::Model<float>::mesh_type());
      std::string name = read<std::string>(stream);
      //std::cout << "  material: " << name << std::endl;
      model->meshes.back().material_index = materials[name];
    } else if (command == "f") { // read a face
      x3d::Model<float>::face_type face;
      for (int i = 0; i < 3; i ++) { // TODO: de-uglify
        // split the vertex definition up into vertex/texcoord/normal (and its variations)
        std::vector<std::string> attrs;
        split(read<std::string>(stream), attrs, "/");
        if (attrs.size() > 0) { // vertex index
          int vertex_index = boost::lexical_cast<int>(attrs[0]) - 1;
          if (vertex_index < 0) { // relative index
            vertex_index = model->vertices.size() + vertex_index + 1;
          }
          face.vertex_index[i] = vertex_index;
          if (vertex_index >= model->vertices.size())
            std::cout << "vi: " << vertex_index << std::endl;
        } if (attrs.size() > 1) { // tex coord index
          int texcoord_index = -1;
          if (attrs[1].empty()) {
            texcoord_index = 0;
          } else {
            texcoord_index = boost::lexical_cast<int>(attrs[1]) - 1;
            if (texcoord_index < 0) { // relative index
              texcoord_index = texcoords.size() + texcoord_index + 1;
            }
          }

          if (texcoord_index >= 0)
            face.texcoord[i] = texcoords[texcoord_index];
        } if (attrs.size() > 2) { // normal index
          int normal_index = boost::lexical_cast<int>(attrs[2]) - 1;
          if (normal_index < 0) { // relative index
            normal_index = normals.size() + normal_index + 1;
          }

          face.normal[i] = normals[normal_index];
        }
        //std::cout << "facevertex: " << face.vertex_index[i] << "/"  << std::endl;
      }
      // add face index to group

      // add a default mesh if there's none present, otherwise add it to the last one added
      if (model->meshes.empty())
        model->meshes.push_back(x3d::Model<float>::mesh_type());

      model->meshes.back().face_indices.push_back(model->faces.size());
      model->faces.push_back(face);
    } else if (command == "mtllib") { // load materials
      boost::filesystem::path material_file_name;
      material_file_name = file_name.branch_path() / read<std::string>(stream);

      if (!boost::filesystem::exists(material_file_name)) {
        throw std::runtime_error("Material file not found");
      }

      boost::filesystem::ifstream matstream(material_file_name);

      if (!matstream.good()) {
        throw std::runtime_error("Cannot read material file");
      }

      std::cout << "reading materials:" << std::endl;
      while (matstream.good()) {
        std::string mat_command;
        mat_command = read<std::string>(matstream);
        if (mat_command == "newmtl") { // create new material
          std::string name = read<std::string>(matstream);
          std::cout << "  material " << name << ":" << std::endl;
          materials[name] = model->materials.size();
          model->materials.push_back(x3d::Material());
        } else if (mat_command == "Ka") { // ambient color
          model->materials.back().ambient.r = read<float>(matstream);
          model->materials.back().ambient.g = read<float>(matstream);
          model->materials.back().ambient.b = read<float>(matstream);
        } else if (mat_command == "Kd") { // diffuse color
          model->materials.back().diffuse.r = read<float>(matstream);
          model->materials.back().diffuse.g = read<float>(matstream);
          model->materials.back().diffuse.b = read<float>(matstream);
        } else if (mat_command == "Ks") { // specular color
          model->materials.back().specular.r = read<float>(matstream);
          model->materials.back().specular.g = read<float>(matstream);
          model->materials.back().specular.b = read<float>(matstream);
        } else if (mat_command == "Ns") { // specular exponent/shininess
          model->materials.back().shininess = read<float>(matstream);
        } else if (mat_command == "map_Kd") { // diffuse map
          std::string texture_name = read<std::string>(matstream);
          model->materials.back().texture.reset(
            m_texture_pool.get(file_name.branch_path() / texture_name)
          );
          std::cout << "    diffuse texture: " << texture_name << std::endl;
        }

        read_line(matstream);
      }
    }
    
    // finish the rest of the line
    read_line(stream);
  }

  return model;
}
