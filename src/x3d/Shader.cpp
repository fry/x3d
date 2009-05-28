#include "x3d/Shader.hpp"

#include <string>
#include <vector>

#include <gl/glew.h>

using namespace x3d;

// Creates a char buffer from a string
std::vector<char> string_to_buffer(const std::string& str) {
  return std::vector<char>(str.c_str(), str.c_str() + str.size() + 1);
}

// Loads a shader and returns its name
unsigned int load_shader(unsigned int program, unsigned int type, const std::string& source) {
  unsigned int shader;
  std::vector<char> buffer = string_to_buffer(source);
  const char* chrs = &buffer.front();
  shader = glCreateShaderObjectARB(type);
  glShaderSourceARB(shader, 1, &chrs, 0);
  glCompileShaderARB(shader);
  glAttachObjectARB(program, shader);
  return shader;
}

// Sets the objects to zero
Shader::Shader(): m_program(0), m_vertex(0), m_fragment(0) {

}

// Deletes the shaders and program objects
Shader::~Shader() {
  delete_shaders();
  if (m_program != 0)
    glDeleteObjectARB(m_program);
}

// Deletes the shaders
void Shader::delete_shaders() {
  if (m_program == 0)
    return;

  if (m_vertex != 0) {
    glDetachObjectARB(m_program, m_vertex);
    glDeleteObjectARB(m_vertex);
    m_vertex = 0;
  }
  if (m_fragment != 0) {
    glDetachObjectARB(m_program, m_fragment);
    glDeleteObjectARB(m_fragment);
    m_fragment = 0;
  }
}

// Deletes the shaders and recreates the program object
void Shader::reset() {
  delete_shaders();
  if (m_program != 0)
    glDeleteObjectARB(m_program);

  m_program = glCreateProgramObjectARB();
  m_log.clear();
}

void Shader::load_fragment(const std::string &contents) {
  if (m_program == 0)
    m_program = glCreateProgramObjectARB();
  m_fragment = load_shader(m_program, GL_FRAGMENT_SHADER_ARB, contents);
}

void Shader::load_vertex(const std::string &contents) {
  if (m_program == 0)
    m_program = glCreateProgramObjectARB();
  m_vertex = load_shader(m_program, GL_VERTEX_SHADER_ARB, contents);
}

void Shader::link() {
  glLinkProgramARB(m_program);

  // Reads the info log
  int length = 0;
  int written = 0;
  char* log;
  glGetObjectParameterivARB(m_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

  if (length > 0) {
    log = new char[length];
    glGetInfoLogARB(m_program, length, &written, log);
    m_log = std::string(log, length);
  }
}

std::string Shader::get_log() {
  return m_log;
}

void Shader::bind() {
  if (m_program == 0 || (m_vertex == 0 && m_fragment == 0))
    throw std::runtime_error("Shader not initialized");
  glUseProgramObjectARB(m_program);
}

void Shader::unbind() {
  glUseProgramObjectARB(0);
}

void Shader::set_uniform_int(const std::string name, int value) {
  if (m_program != 0) 
    glUniform1iARB(glGetUniformLocationARB(m_program, name.c_str()), value);
}

void Shader::set_uniform_float(const std::string name, float value) {
  if (m_program != 0) 
    glUniform1fARB(glGetUniformLocationARB(m_program, name.c_str()), value);
}
