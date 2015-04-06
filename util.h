#include <GL/glew.h>

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader);
GLuint load_shader(GLenum type, const char* filepath);
