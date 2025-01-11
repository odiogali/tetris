#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n" // position variable has attribute pos 0
  "layout (location = 1) in vec3 aColor;\n" // color variable has attribute pos 1
  "out vec3 ourColor;\n" // output a color to the fragment shader
  "void main(){\n"
  " gl_Position = vec4(aPos, 1.0);\n"
  " ourColor = aColor;\n"
  "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n"
  "void main(){\n"
  " FragColor = vec4(ourColor, 1.0);\n"
  "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0,width,height);
}

void processInput(GLFWwindow* window){
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Tetris", NULL, NULL);
  if (window == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // x = 0 starts from left, y = 0 starts from bottom
  glViewport(0,0,800,600); // glViewPort(x, y, width, height)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
    // vertices         // Colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
  };

  // build and compile vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile-time error regarding the vertex shader
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(vertexShader, 512,  NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // build and compile fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compilation errors for the fragment shader
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragmentShader, 512,  NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // link the shaders into one shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check if linking the shader program has failed
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success){
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::LINKING::LINK_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  unsigned int VBO;
  glGenBuffers(1, &VBO); // generate a single vertex buffer object and store the object ID in VBO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // Copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // we are to work on the 'VBO' with the object ID in VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set our vertex attributes pointers
  // Param 1: specifies which vertex attrib we want to configure - we specified location of position vertex attribute
  // in vertex shader with 'layout (location = 0)' - sets the location of the vertex attrib to 0; since we want to 
  // pass data to this vertex attribute, we pass in 0
  // Param 2: specifies the size of the vertex attrib - a vec3 thus it is composed of 3 values
  // Param 3: specifies the type of the data: GL_FLOAT (a vec* in GLSL consists of floating point values)
  // Param 5: known as stride and tells us the space between consecutive vertex attrib - 3 times the size of a float
  // but we could've also just used 0 since the vertices are tightly packed
  // Param 6: the offset of where the position data begins in the buffer - since pos data is at the start of the data
  // array, this value is just 0. The type of this parameter is void*
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind registered VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Unbind registered VAO so that other VAO calls don't accidentally modify this VAO; in general, don't unbind VAOs
  // or VBOs unless directly necessary
  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window)){
    processInput(window);    

    // Draw blue background
    glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Deallocate used resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
