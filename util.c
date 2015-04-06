#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "log.h"
#include "util.h"

static char* load_file(const char* filepath)
{
        FILE* file = fopen(filepath, "r");
        fseek(file, 0, SEEK_END);
        size_t length = (size_t) ftell(file);
        rewind(file);

        char* content = malloc((length + 1)* sizeof(char));
        content[length] = '\0';
        size_t read_length = fread(content, sizeof(char), length, file);
        
        if (read_length != length) {
                log_warn("Size of file %s changed during read.\n", filepath);
        }
        fclose(file);
        return content;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader)
{
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        log_program_info(program);

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        return program;
}

GLuint load_shader(GLenum type, const char* filepath)
{
        log_debug("Loading shader '%s'\n", filepath);
        const char* shader_data = load_file(filepath);

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shader_data, NULL);

        log_debug("Compiling shader '%s'\n", filepath);
        glCompileShader(shader);
        log_shader_info(shader);

        free((char*)shader_data);
        return shader;
}

