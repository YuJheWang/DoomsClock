#ifndef MYSHADER_H
#define MYSHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>

class Shader {
public:

    GLuint id = 0;

    typedef enum Type { Vertex, Fragment, Geometry, Total } Type;

    struct {
        GLuint id;
        std::string source;
        GLenum type;
    } shaders[Total] = {
        { 0, "", GL_VERTEX_SHADER },
        { 0, "", GL_FRAGMENT_SHADER },
        { 0, "", GL_GEOMETRY_SHADER }
    };

    //Constructor&destructor

    Shader() {}

    Shader(const char *vsfile, const char *fsfile, const char *gsfile = "") {
        loadFromFile(vsfile, fsfile, gsfile);
    }

    Shader(const Shader& other)
    {
        std::copy(other.shaders, other.shaders + Total, shaders);
        id = other.id; linked = other.linked;
    }

    ~Shader() {
         if (id) glDeleteProgram(id);
    }

    //Loading functions.

    void loadFromSource(std::string shaderSource, Type shaderType) {
        if (shaderType > 2) std::cout << "ERROR::SHADER::Wrong shader type!" << std::endl;

        shaders[shaderType].id = glCreateShader(shaders[shaderType].type);

        shaders[shaderType].source = shaderSource;
        const char *src = shaderSource.c_str();

        GLuint shaderId = shaders[shaderType].id;

        int success; char infoLog[512];

        glShaderSource(shaderId, 1, &src, NULL);
        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::Fail to compile shader!, info log: " << infoLog << std::endl;
        }
    }

    void loadFromFile(const char *vsfile, const char *fsfile, const char *gsfile = "") {
        loadFromFile(vsfile, Vertex);
        loadFromFile(fsfile, Fragment);
        if (gsfile != "")
            loadFromFile(gsfile, Geometry);
    }

    void loadFromFile(const char *fileName, Type shaderType) {
        loadFromSource(readFile(fileName), shaderType);
    }

    void use() {
        if (!linked) {
            createAndLink();
            linked = true;
        }

        glUseProgram(id);
    }

    void unuse() {
        glUseProgram(0);
    }

    void setUniform(const char *name, int value) {
        glProgramUniform1i(id, indexOf(name), value);
    }

    void setUniform(const char *name, float value) {
        glProgramUniform1f(id, indexOf(name), value);
    }

    void setUniform(const char *name, double value) {
        glProgramUniform1d(id, indexOf(name), value);
    }

    void setUniform(const char *name, bool value) {
        glProgramUniform1i(id, indexOf(name), value);
    }

    
    void setUniform(const char *name, const glm::vec2& vec) {
        glProgramUniform2fv(id, indexOf(name), 1, glm::value_ptr(vec));
    }

    void setUniform(const char *name, const glm::vec3& vec) {
        glProgramUniform3fv(id, indexOf(name), 1, glm::value_ptr(vec));
    }

    void setUniform(const char *name, const glm::vec4& vec) {
        glProgramUniform4fv(id, indexOf(name), 1, glm::value_ptr(vec));
    }

    void setUniform(const char *name, const glm::mat2& mat) {
        glProgramUniformMatrix2fv(id, indexOf(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setUniform(const char *name, const glm::mat3& mat) {
        glProgramUniformMatrix3fv(id, indexOf(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setUniform(const char *name, const glm::mat4& mat) {
        glProgramUniformMatrix4fv(id, indexOf(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:

    bool linked = false;

    std::string readFile(const char *filename) {
        std::fstream file(filename);
        std::string result, temp;

        if (file.is_open()) {
            while (std::getline(file, temp)) {
                result += temp + '\n';
            }
        } else {
            std::cout << "ERROR::SHADER::Cannot open the file!" << std::endl; 
        }

        return (result);
    }

    void createAndLink() {
        GLuint vs, fs, gs;

        vs = shaders[Vertex].id; 
        fs = shaders[Fragment].id; 
        gs = shaders[Geometry].id;

        id = glCreateProgram();

        glAttachShader(id, vs);
        glAttachShader(id, fs);
        if (gs) glAttachShader(id, gs);

        glLinkProgram(id);
        int success; char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cerr << "Fail to link program! info log : \n" << infoLog << std::endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
        if (gs) glDeleteShader(gs);
    }

    inline int indexOf(const char *name) {
        return (glGetUniformLocation(id, name));
    }

};

#endif