#include "ShadersConf.h"

ShadersConf::ShadersConf() : vertex_shader_template("#version 130\n"
                                                    "in vec3 position;\n"
                                                    "in vec3 color;\n"
                                                    "out vec3 fragColor;\n"
                                                    "void main() {\n"
                                                    "   gl_Position = vec4(position, 1.0);\n"
                                                    "   fragColor = color;\n"
                                                    "}\n"),
                             fragment_shader_template("#version 130\n"
                                                      "in vec3 fragColor;\n"
                                                      "out vec4 finalColor;\n"
                                                      "void main() {\n"
                                                      "   finalColor = vec4(fragColor, 1.0);\n"
                                                      "}\n"),
                             conf_id(0)
{
}

GLuint ShadersConf::getConfId() const
{
    return this->conf_id;
}

void ShadersConf::enable() const
{
    glUseProgram(this->conf_id);
}

void ShadersConf::disable() const
{
    glUseProgram(0);
}

GLuint ShadersConf::getUniformVarId(std::string uniform_var_name) const
{
    return this->uniform_vars_ids.at(uniform_var_name);
}

bool ShadersConf::prepareShaders(GLenum shader_type, GLuint *shader_id, std::string shader_template)
{
    GLuint shader;
    const char *shader_template_cstr;
    int compilationSuccess;

    shader_template_cstr = shader_template.c_str();

    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_template_cstr, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
    if (!compilationSuccess)
    {
        char infoLog[512];

        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        switch (shader_type)
        {
        case GL_VERTEX_SHADER:
            std::cerr << "Vertex shader compilation failed:\n"
                      << infoLog << std::endl;
            break;
        case GL_FRAGMENT_SHADER:
            std::cerr << "Fragment shader compilation failed:\n"
                      << infoLog << std::endl;
            break;
        }

        glDeleteShader(shader);

        return false;
    }

    *shader_id = shader;

    return true;
}

bool ShadersConf::prepareShadersProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id)
{
    GLuint program;

    int linkingSuccess;

    program = glCreateProgram();

    if (vertex_shader_id)
        glAttachShader(program, vertex_shader_id);

    if (fragment_shader_id)
        glAttachShader(program, fragment_shader_id);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linkingSuccess);
    if (!linkingSuccess)
    {
        char infoLog[512];

        glGetProgramInfoLog(program, 512, NULL, infoLog);

        std::cerr << "Shader program linking failed: \n"
                  << infoLog << std::endl;

        glDeleteProgram(program);

        return false;
    }

    this->conf_id = program;
    return true;
}

bool ShadersConf::init()
{
    GLuint vertex_shader;
    GLuint fragment_shader;

    bool execution_status;

    execution_status = this->prepareShaders(GL_VERTEX_SHADER, &vertex_shader, this->vertex_shader_template);

    if (!execution_status)
    {
        return false;
    }

    execution_status = this->prepareShaders(GL_FRAGMENT_SHADER, &fragment_shader, this->fragment_shader_template);

    if (!execution_status)
    {
        return false;
    }

    execution_status = this->prepareShadersProgram(vertex_shader, fragment_shader);

    if (!execution_status)
    {
        return false;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    this->setupUniformVars();

    return true;
}

void ShadersConf::setupUniformVars()
{
    const GLsizei max_uniform_var_size = 64;
    GLchar name[max_uniform_var_size];
    GLsizei unifomr_var_name_length;
    GLint number_of_uniforms;
    GLint uniform_var_size;
    GLenum uniform_var_type;

    glGetProgramiv(this->conf_id, GL_ACTIVE_UNIFORMS, &number_of_uniforms);

    for (GLint i = 0; i < number_of_uniforms; i++)
    {
        glGetActiveUniform(this->conf_id, i, max_uniform_var_size, &unifomr_var_name_length, &uniform_var_size, &uniform_var_type, name);

        this->uniform_vars_ids[name] = glGetUniformLocation(this->conf_id, name);
    }
}