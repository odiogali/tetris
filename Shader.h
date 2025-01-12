#ifndef SHADER_H // tells compiler to only include these files if they haven't been included yet
#define SHADER_H // so that if several files include these header files, there won't be linking conflicts

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
  unsigned int ID; // program ID

  // Constructor to read and build the shader
  Shader(const char* vertexPath, const char* fragmentPath);

  // use and activate the shader
  void use();

  // utility and uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
