#ifndef GKOM2024L_SHADERSCONF_H
#define GKOM2024L_SHADERSCONF_H

#include <GL/glew.h>
#include <map>
#include <iostream>

class ShadersConf
{
private:
    const std::string vertex_shader_template;
    const std::string fragment_shader_template;
    GLuint conf_id;
    std::map<std::string, GLuint> uniform_vars_ids;
    bool prepareShaders(GLenum shader_type, GLuint *shader_id, std::string shader_template);
    bool prepareShadersProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id);
    void setupUniformVars();

public:
    GLuint getConfId() const;
    GLuint getUniformVarId(std::string uniform_var_name) const;
    bool init();
    ShadersConf();
    ShadersConf(std::string vertex_shader_template, std::string fragment_shader_template);
    ~ShadersConf() = default;
    ShadersConf(const ShadersConf &) = delete;
    ShadersConf &operator=(const ShadersConf &) = delete;
    void enable() const;
    void disable() const;
};

#endif