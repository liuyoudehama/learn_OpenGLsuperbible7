// Include the "sb7.h" header file
#include "sb7.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <memory>

std::string read_shader_code_from_file(const char *file_path);
GLuint compile_shaders(void);

// Derive my_application from sb7::application
class my_application : public sb7::application
{
private:
    GLuint rendering_program;
    GLuint vertex_array_object;

public:
    void startup()
    {
        rendering_program = compile_shaders();
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void shutdown()
    {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vertex_array_object);
    }

public:
    // Our rendering function
    void render(double currentTime)
    {
        // Simply clear the window with red
        static const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, red);

        glUseProgram(rendering_program);

        glDrawArrays(GL_POINTS, 0, 1);

        glPointSize(40.0f);
    }
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);




std::string read_shader_code_from_file(const char *file_path)
{
    std::string shader_code;
    std::ifstream shader_file_stream(file_path, std::ios::in);
    if(shader_file_stream.is_open())
    {
        std::stringstream buf;
        buf << shader_file_stream.rdbuf();
        shader_file_stream.close();
        shader_code = buf.str();
    }
    return shader_code;
}

GLuint compile_shaders(void)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    std::string vertex_shader_code = read_shader_code_from_file("../shaders/playground.vert.glsl");
    const char* p_vertex_shader = vertex_shader_code.c_str();
    std::string fragment_shader_code = read_shader_code_from_file("../shaders/playground.frag.glsl");
    const char* p_fragment_shader = fragment_shader_code.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &p_vertex_shader, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &p_fragment_shader, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}