#version 330 core
layout(location = 0) in vec3 aPos; // position variable has attribute pos 0
layout(location = 1) in vec3 aColor; // color variable has attribute pos 1
uniform float offset;

out vec3 ourColor; // output a color to the fragment shader

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aPos;
}
