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

// Derive my_application from sb7::application
class my_application : public sb7::application
{
private:
    GLuint rendering_program;
    GLuint vertex_array_object;
    std::vector<GLuint> m_application_shaders;

private:
    bool is_successful_compiled(GLuint shader, GLenum shader_type)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if(isCompiled == GL_FALSE)
        {
            std::cout << "----Failed to compile shader----" << std::endl;
            std::cout << "shader number:" << shader << std::endl;
            std::cout << "shader type:" << shader_type << std::endl;

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

            std::cout << "----Failed to compile shader end----" << std::endl;

            return false;
        }
        return true;
    }

    bool is_successful_linked(GLuint program)
    {
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            std::cout << "---- Failed to link program ----" << std::endl;
            std::cout << "program:" << program << std::endl;
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            for(auto ch : infoLog) std::cout << ch;
            std::cout << std::endl;

            // The program is useless now. So delete it.
            glDeleteProgram(program);

            // Provide the infolog in whatever manner you deem best.
            // Exit with failure.
            std::cout << "---- Failed to link program  end ----" << std::endl;

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

        if(!is_successful_compiled(shader, shader_type)) return -1;
        return shader;
    }

    void load_and_attach_shader(const char* shader_path, GLenum shader_type, GLuint program_to_link)
    {
        std::cout << "compiling shader:" << std::endl;
        std::cout << "type:" << shader_type << std::endl;
        GLuint shader = compile_shaders_from_file(shader_path, shader_type);
        if(shader == -1) std::cout << "failed." << std::endl;
        glAttachShader(program_to_link, shader);
        m_application_shaders.push_back(shader);
    }

    GLuint compile_shaders(void)
    {
        GLuint program;
        program = glCreateProgram();

        load_and_attach_shader("../shaders/playground.vert.glsl", GL_VERTEX_SHADER, program);
        load_and_attach_shader("../shaders/playground.frag.glsl", GL_FRAGMENT_SHADER, program);
        // load_and_attach_shader("../shaders/playground.tcs.glsl", GL_TESS_CONTROL_SHADER, program);
        // load_and_attach_shader("../shaders/playground.tes.glsl", GL_TESS_EVALUATION_SHADER, program);

        glLinkProgram(program);
        glValidateProgram(program);

        for(auto shader_id: m_application_shaders)
        {
            glDeleteShader(shader_id);
        }

        return is_successful_linked(program) ? program : 0;
    }

public:
    void startup()
    {
        rendering_program = compile_shaders();

        float positions[6] = {
            -0.5f, -0.5f,
            0.0f, 0.0f,
            0.5f, -0.5f,
        };

        unsigned int buffer_id;

        glGenBuffers(1, &buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

        for (int i = 0; i < 16; i++)
        {
            glEnableVertexAttribArray(0);
        }
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        const GLubyte* glVersion = glGetString(GL_VERSION);
        std::cout << "OpenGL Version: " << glVersion << std::endl;
    }

    // Our rendering function
    void render(double currentTime)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glDrawElements(GL_TRIANGLES, 3, )
    }

    void shutdown()
    {      
        
    }
};

// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);