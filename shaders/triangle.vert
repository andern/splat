#version 450

in vec3 vp;

uniform mat4 view, proj;

void main() {
    gl_Position = view * vec4 (vp, 1);
}
