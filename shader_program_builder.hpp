#pragma once

#include "shader_program.hpp"

class ShaderProgramBuilder{
public:
    ShaderProgramBuilder() { m_programId = glCreateProgram(); }

    ShaderProgramBuilder& withVertexShader(std::string filePath) { return withShader(filePath, "VERTEX", GL_VERTEX_SHADER); }
    ShaderProgramBuilder& withFragmentShader(std::string filePath) { return withShader(filePath, "FRAGMENT", GL_FRAGMENT_SHADER); }
    ShaderProgramBuilder& withGeometryShader(std::string filePath) { return withShader(filePath, "GEOMETRY", GL_GEOMETRY_SHADER); }

    ShaderProgram build() { 
        glLinkProgram(m_programId);
        checkCompileErrors(m_programId, "PROGRAM");
        return ShaderProgram(m_programId); 
    }
private:
    unsigned int m_programId;

    ShaderProgramBuilder& withShader(std::string filePath, std::string shaderType, unsigned int glShaderConst) {
        std::string code;
        code = FileReader::read(filePath);
        const char* shaderCode = code.c_str();

        unsigned int shaderId;
	
        shaderId = glCreateShader(glShaderConst);
        glShaderSource(shaderId, 1, &shaderCode, NULL);
        glCompileShader(shaderId);
        checkCompileErrors(shaderId, shaderType);
        
        glAttachShader(m_programId, shaderId);
        glDeleteShader(shaderId);
        return *this;
    }
    
    void checkCompileErrors(GLuint shader, std::string type) const
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
