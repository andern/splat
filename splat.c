/* 
 * Copyright (C) 2015 Andreas Halle
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "gl_math.h"
#include "log.h"
#include "util.h"

#define AUTHORS "Andreas Halle"

float points[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
};

int w_width = 800;
int w_height = 600;

static void version(void)
{
        fprintf(stdout, "\
" PROGRAM_NAME " " VERSION "\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
\n\
Written by " AUTHORS ".\n\
");
        exit(0);
}

static void usage(int status)
{
        if (status != 0) {
                fprintf(stderr, "\
Try '" PROGRAM_NAME " --help' for more information.\n");
        } else {
                printf("usage: splat\n");
                printf("\
For an OpenGL tutorial.\n\
      --help     display this help and exit\n\
      --version  output version information and exit\n\
");
        }
        exit(status);
}

static void error_callback(int error, const char* description)
{
        log_error("GLFW: %d %s\n", error, description);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
        w_width = width;
        w_height = height;
        // TODO: Update perspective matrix
}

static void update_fps_counter(GLFWwindow* window)
{
        static double prev;
        static int frame_count;

        double current = glfwGetTime();
        double elapsed = current - prev;

        if (elapsed >= 0.25) {
                prev = current;
                double fps = frame_count / elapsed;

                char buf[64];
                sprintf(buf, "fps: %.1f", fps);
                glfwSetWindowTitle(window, buf);
                frame_count = 0;
        }
        frame_count++;
}

float cam_pos[] = {0.0f, 0.0f, 1.0f};
float cam_yaw = 0.0f;
float cam_pitch = 0.0f;
float cam_speed = 0.1f;
float cam_yaw_speed = 10.0f;

float near = 0.1f;
float far = 100.0f;

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
        printf("%f %f\n", xpos, ypos);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pos[2] -= cam_speed;
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pos[0] -= cam_speed;
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pos[2] += cam_speed;
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pos[0] += cam_speed;
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_yaw -= cam_yaw_speed;
        if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_yaw += cam_yaw_speed;
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pitch -= cam_yaw_speed;
        if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
                cam_pitch += cam_yaw_speed;
}

int main(int argc, char* argv[])
{
        static struct option const long_options[] =
        {
                {"help", no_argument, NULL, 'h'},
                {"version", no_argument, NULL, 'v'},
                {NULL, 0, NULL, 0}
        };

        int c;
        while ((c = getopt_long (argc, argv, "hv", long_options, NULL))
                        != -1) {
                switch (c) {
                case 'h':
                        usage(0);
                case 'v':
                        version();
                default:
                        usage(1);
                }
        }
        log_open("splat.log", LOG_DEBUG);
        glfwSetErrorCallback(error_callback);

        log_info("Initializing GLFW version %s\n", glfwGetVersionString());
        if (!glfwInit()) {
                log_error("Could not start GLFW\n");
                return EXIT_FAILURE;
        }

        log_debug("Opening GLFW window of size %d x %d\n",w_width,w_height);
        GLFWwindow* window = glfwCreateWindow(
                        w_width, w_height, "Splat", NULL, NULL);
        if (!window) {
                log_error("Could not open GLFW window\n");
                log_error("Terminating GLFW\n");
                glfwTerminate();
                return EXIT_FAILURE;
        }

        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_pos_callback);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        log_info("Initializing GLEW version %s\n", glewGetString(GLEW_VERSION));
        if (err != GLEW_OK) {
                log_error("Could not initialize GLEW\n");
                log_error("GLEW error: %s\n", glewGetErrorString(err));
        }

        log_info("Renderer: %s\n", glGetString(GL_RENDERER));
        log_info("OpenGL version supported: %s\n", glGetString(GL_VERSION));
        log_gl_params();

        /* Only draw pixels on shapes that are close to the viewer */
//        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LESS);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);
        
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        GLuint vs = load_shader(GL_VERTEX_SHADER, "shaders/triangle.vert");
        GLuint fs = load_shader(GL_FRAGMENT_SHADER, "shaders/color.frag");
        GLuint sp = create_program(vs, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        float fov = rad(90.0f);
        float aspect = (float)w_width / (float)w_height;
        float range = tanf(fov * 0.5f) * near;
        float Sx = (2.0f * near) / (range * aspect + range * aspect);
        float Sy = near / range;
        float Sz = -(far + near) / (far - near);
        float Pz = -(2.0f * far * near) / (far - near);

        mat4 proj_mat = {0};
        mat4 view_mat = {0};
        mat4 world_mat = {0};
        mat_translate(world_mat, 1, 0, 0);

        mat_diag(proj_mat, Sx, Sy, Sz, 0);
        proj_mat[11] = -1;
        proj_mat[14] = Pz;

        mat4 T = {0};
        mat4 R = {0};
        mat4 Ry = {0};
        mat4 Rx = {0};

        int proj_loc = glGetUniformLocation(sp, "proj");
        int view_loc = glGetUniformLocation(sp, "view");
        int world_loc = glGetUniformLocation(sp, "world");

        while (!glfwWindowShouldClose(window)) {
                glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                mat_translate(T, -cam_pos[0], -cam_pos[1], -cam_pos[2]);
                mat_rotate_y(Ry, -cam_yaw);
                mat_rotate_x(Rx, -cam_pitch);
                mat_mul(R, Ry, Rx);
                mat_mul(view_mat, R, T);

                glUseProgram(sp);
                glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj_mat);
                glUniformMatrix4fv(view_loc, 1, GL_FALSE, view_mat);
                glUniformMatrix4fv(world_loc, 1, GL_FALSE, world_mat);

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glfwSwapBuffers(window);

                update_fps_counter(window);
                glfwPollEvents();
        }

        glDeleteProgram(sp);
        glfwDestroyWindow(window);
        glfwTerminate();
        log_close();
        return EXIT_SUCCESS;;
}

