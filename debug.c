#include <GL/glew.h>

#include "debug.h"
#include "log.h"

static GLenum gl_params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO
};

static const char* gl_names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO"
};

void log_gl_params(void)
{
        log_debug("GL Context Parameters:\n");
        for (int i = 0; i < 10; i++) {
                int v = 0;
                glGetIntegerv(gl_params[i], &v);
                log_debug("%s = %d\n", gl_names[i], v);
        }

        int v[2];
        glGetIntegerv(gl_params[10], v);
        log_debug("%s = %d %d\n", gl_names[10], v[0], v[1]);

        unsigned char s = 0;
        glGetBooleanv(gl_params[11], &s);
        log_debug("%s %u\n", gl_names[11], s);
}

void log_shader_info(GLuint shader)
{
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success == GL_TRUE) return;

        int length;
        char buf[2048];
        glGetShaderInfoLog(shader, 2048, &length, buf);
        log_error("Shader on index %u did not compile:\n%s\n", shader, buf);
}

void log_program_info(GLuint program)
{
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (success == GL_TRUE) return;

        int length;
        char buf[2048];
        glGetProgramInfoLog(program, 2048, &length, buf);
        log_error("Could not link program on index %u:\n%s\n", program, buf);
}
