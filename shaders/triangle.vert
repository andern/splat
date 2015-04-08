#version 450

in vec3 vp;

uniform mat4 world, view, proj;

void main() {
    gl_Position = proj * view * world * vec4 (vp, 1);
}
