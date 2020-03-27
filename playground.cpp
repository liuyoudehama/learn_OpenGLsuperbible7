// Include the "sb7.h" header file
#include "sb7.h"
#include <math.h>

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
        static const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f , 1.0f};
        glClearBufferfv(GL_COLOR, 0, red);

        glUseProgram(rendering_program);

        glDrawArrays(GL_POINTS, 0, 1);

        glPointSize(40.0f);
    }
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);


GLuint compile_shaders(void)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar * vertex_shader_source[] = 
    {
        "#version 450 core                                  \n"
        "                                                   \n"
        "void main(void)                                    \n"
        "{                                                  \n"
        "       gl_Position = vec4(0.0, 0.0, 0.5, 1.0);     \n"
        "}                                                  \n"
    };

    static const GLchar * fragment_shader_source[] = 
    {
        "#version 450 core                                  \n"
        "                                                   \n"
        "out vec4 color;                                    \n"
        "                                                   \n"
        "void main(void)                                    \n"
        "{                                                  \n"
        "       color = vec4(0.0, 0.8, 0.5, 1.0);           \n"
        "}                                                  \n"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}