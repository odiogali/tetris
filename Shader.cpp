#include "Shader.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath){
  // 1. Retrive the vertex/fragment source code from filePath
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try{
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;

    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure e){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // 2. Compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // build and compile vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  // Check for compile-time error regarding the vertex shader
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(vertex, 512,  NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // build and compile fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  // Check for compilation errors for the fragment shader
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragment, 512,  NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // link the shaders into one shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // Check if linking the shader program has failed
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success){
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::LINKING::LINK_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
};

void Shader::use(){
  glUseProgram(ID);
};

// utility and uniform functions
void Shader::setBool(const std::string &name, bool value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
};
void Shader::setInt(const std::string &name, int value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};
void Shader::setFloat(const std::string &name, float value) const{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
};
