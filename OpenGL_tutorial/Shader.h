#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int vertexId, fragmentId, tessControlId, tessEvalId, geometryId, computeId;
    unsigned int MAX_SHADERS = 6;
    unsigned int programId;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() {
        glDeleteProgram(programId);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(programId);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setUniformBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setUniformInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setUniformFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }

// adds shader to program, does not compile the actual prgoram
//-------------------------------------------------------------------------------
    void addShader(const char* shaderFilePath, GLenum shaderType);
    // links and compiles shader program
//--------------------------------------------
    void linkAndCompile();

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkForCompileErrors(unsigned int shader, std::string type);

// compiles and attaches a shader to the program
//-------------------------------------------------
    void attachShader(GLenum shaderType, const char* shaderSource);
};
#endif

