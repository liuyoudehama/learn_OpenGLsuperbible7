// Include the "sb7.h" header file
#include "sb7.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

std::string read_shader_code_from_file(const char *file_path);
GLuint compile_shaders(void);
GLuint compile_shaders_from_file(const char *file_path, GLenum shader_type);
bool is_successful_compiled(GLuint shader);

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}

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
        static const GLfloat red[] = { 0.0f, 0.0f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, red);
        
        glUseProgram(rendering_program);

        GLfloat attrib[] =
        {
            (float)sin(currentTime) * 0.5f,
            (float)cos(currentTime) * 0.6f,
            0.0f,
            0.0f
        };

        GLfloat current_color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f};

        glVertexAttrib4fv(0, attrib);
        glVertexAttrib4fv(1, current_color);

        GLClearError();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GLCheckError();
    }
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);


bool is_successful_compiled(GLuint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.

        for(auto ch : errorLog) std::cout << ch;
        std::cout << std::endl;

        glDeleteShader(shader); // Don't leak the shader.
        return false;
    }
    return true;
}

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
    else
    {
        printf("shader file: %s not found\n", file_path);
    }
    return shader_code;
}

GLuint compile_shaders_from_file(const char *file_path, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    std::string shader_code = read_shader_code_from_file(file_path);
    const char *p_shader_code = shader_code.c_str();
    glShaderSource(shader, 1, &p_shader_code, NULL);
    glCompileShader(shader);

    if(!is_successful_compiled(shader)) return -1;
    return shader;
}

GLuint compile_shaders(void)
{
    GLuint program;

    std::cout << "compiling vertex shader:" << std::endl;
    GLuint vertex_shader = compile_shaders_from_file("../shaders/playground.vert.glsl", GL_VERTEX_SHADER);
    if(vertex_shader == -1) std::cout << "failed." << std::endl;
    
    std::cout << "compiling fragment shader:" << std::endl;
    GLuint fragment_shader = compile_shaders_from_file("../shaders/playground.frag.glsl", GL_FRAGMENT_SHADER);
    if(vertex_shader == -1) std::cout << "failed." << std::endl;
    
    std::cout << "compiling tcs shader:" << std::endl;
    GLuint tcs_shader = compile_shaders_from_file("../shaders/playground.tcs.glsl", GL_TESS_CONTROL_SHADER);
    if(tcs_shader == -1) std::cout << "failed." << std::endl;

    std::cout << "compiling tes shader:" << std::endl;
    GLuint tes_shader = compile_shaders_from_file("../shaders/playground.tes.glsl", GL_TESS_EVALUATION_SHADER);
    if(tes_shader == -1) std::cout << "failed." << std::endl;

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glAttachShader(program, tcs_shader);
    glAttachShader(program, tes_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}