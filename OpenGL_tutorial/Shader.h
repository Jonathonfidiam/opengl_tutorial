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
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        vertexId, fragmentId, tessControlId, tessEvalId, geometryId, computeId = 0;
        programId = glCreateProgram();
        addShader(vertexPath, GL_VERTEX_SHADER);
        addShader(fragmentPath, GL_FRAGMENT_SHADER);
    }
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
    void addShader(const char* shaderFilePath, GLenum shaderType) {
        std::string shaderSource;
        std::ifstream shaderFile;
        std::stringstream shaderStream;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(shaderFilePath);
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderSource = shaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        attachShader(shaderType, shaderSource.c_str());
    }
    // links and compiles shader program
//--------------------------------------------
    void linkAndCompile() {
        glLinkProgram(programId);

        // cleanup
        unsigned int shaderProgramIds[6] = { vertexId, fragmentId, tessControlId, tessEvalId, geometryId, computeId };
        for (unsigned int i = 0; i < MAX_SHADERS; i++) {
            std::cout << "Shdaer Id: " << shaderProgramIds[i] << std::endl;
            if (shaderProgramIds[i] != 0) {
                glDetachShader(programId, shaderProgramIds[i]);
                glDeleteShader(shaderProgramIds[i]);

            }
        }
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkForCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
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


// compiles and attaches a shader to the program
//-------------------------------------------------
    void attachShader(GLenum shaderType, const char* shaderSource) {

        switch (shaderType) {
        case GL_VERTEX_SHADER:
            vertexId = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexId, 1, &shaderSource, 0);
            glCompileShader(vertexId);
            glAttachShader(programId,vertexId);
            checkForCompileErrors(vertexId, "VERTEX");
            break;
        case GL_FRAGMENT_SHADER:
            fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentId, 1, &shaderSource, 0);
            glCompileShader(fragmentId);
            glAttachShader(programId, fragmentId);
            checkForCompileErrors(fragmentId, "FRAGMENT");
            break;
        case GL_GEOMETRY_SHADER:
            geometryId = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometryId, 1, &shaderSource, 0);
            glCompileShader(geometryId);
            glAttachShader(programId, geometryId);
            checkForCompileErrors(geometryId, "GEOMETRY");
            break;
        case GL_TESS_CONTROL_SHADER:
            tessControlId = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tessControlId, 1, &shaderSource, 0);
            glCompileShader(tessControlId);
            glAttachShader(programId, tessControlId);
            checkForCompileErrors(tessControlId, "TESS_CONTROL");
            break;
        case GL_TESS_EVALUATION_SHADER:
            tessEvalId = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tessEvalId, 1, &shaderSource, 0);
            glCompileShader(tessEvalId);
            glAttachShader(programId, tessEvalId);
            checkForCompileErrors(tessEvalId, "TESS_EVAL");
            break;
        case GL_COMPUTE_SHADER:
            computeId = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(computeId, 1, &shaderSource, 0);
            glCompileShader(computeId);
            glAttachShader(programId, computeId);
            checkForCompileErrors(computeId, "COMPUTE");
            break;
        default:
            std::cout << "ERROR: A valid shader type was not entered\n";
            break;
        }
    }
};
#endif

