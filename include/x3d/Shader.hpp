#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>

namespace x3d {
  // An OpenGL shader class
  class Shader {
  protected:
    // Stores the name of the program object
    unsigned int m_program;
    // Stores vertex and fragment shader name
    unsigned int m_vertex;
    unsigned int m_fragment;

    // Contains the info log (if it exists);
    std::string m_log;
  public:
    Shader();
    ~Shader();
    // Dettaches and deletes used shaders
    void delete_shaders();
    // Deletes shaders and recreates the program object
    void reset();

    // Creates and compiles the shader to the source in the parameter
    void load_vertex(const std::string& contents);
    void load_fragment(const std::string& contents);

    // Links the shaders to the program object
    void link();

    // Returns a string with the info log
    std::string get_log();

    // Bind/unbind the shader
    void bind();
    void unbind();

    // Set uniform variable
    void set_uniform_int(const std::string name, int value);
    void set_uniform_float(const std::string name, float value);
  };
}

#endif