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
    unsigned int programId;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        // ensure ifstream objects can throw exceptions:
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            std::stringstream vertexShaderStream, fragmentShaderStream;
            // read file's buffer contents into streams
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream into string
            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertexShader, fragmentShader;
        // vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        checkForCompileErrors(vertexShader, "VERTEX");
        // fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        checkForCompileErrors(fragmentShader, "FRAGMENT");
        // shader Program
        shaderProgramId = glCreateProgram();
        glAttachShader(shaderProgramId, vertexShader);
        glAttachShader(shaderProgramId, fragmentShader);
        glLinkProgram(shaderProgramId);
        checkForCompileErrors(shaderProgramId, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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
        glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setUniformInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setUniformFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgramId, name.c_str()), value);
    }

// checks if shader type is a valid shader
//-------------------------------------------------------------------------------
    bool isValidShaderType(GLenum shaderType) {
        if (shaderType == GL_VERTEX_SHADER ||
            shaderType == GL_FRAGMENT_SHADER ||
            shaderType == GL_GEOMETRY_SHADER ||
            shaderType == GL_TESS_CONTROL_SHADER ||
            shaderType == GL_TESS_EVALUATION_SHADER ||
            shaderType == GL_COMPUTE_SHADER ||
            ) {
            return true;
        }
        else {
            return false;
        }
    }

// adds shader to program, does not compile the actual prgoram
//-------------------------------------------------------------------------------
    void addShader(const char* shaderFilePath, GLenum shaderType) {
        std::string shaderCode;
        std::ifstream shaderFileBuffer;
        shaderFileBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            shaderFileBuffer.open(shaderFilePath);
            std::stringstream shaderStringStream;
            // read file's buffer contents into streams
            shaderStringStream << shaderFileBuffer.rdbuf();

            // close file handlers
            shaderFileBuffer.close();
            // convert stream into string
            shaderCode = shaderStringStream.str();

        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* shaderCode_cstr = shaderCode.c_str();
        // 2. compile shaders
        unsigned int shaderId;
        // vertex shader
        switch (shaderType) {
        case GL_VERTEX_SHADER:
            vertexId = glCreateShader(GL_VERTEX_SHADER);
            std::string shaderSource = getShaderSource(shaderFilePath);

            break;
        }

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        checkForCompileErrors(vertexShader, "VERTEX");
        // fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        checkForCompileErrors(fragmentShader, "FRAGMENT");
        // shader Program
        shaderProgramId = glCreateProgram();
        glAttachShader(shaderProgramId, vertexShader);
        glAttachShader(shaderProgramId, fragmentShader);
        glLinkProgram(shaderProgramId);
        checkForCompileErrors(shaderProgramId, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

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

// returns the shader source as a string
 //-----------------------------------------------------------------
    std::string getShaderSource(const char* shaderFilePath) {
        const char* shaderSource;
        std::ifstream shaderFile;
        std::stringstream shaderStream;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(shaderFilePath);
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderSource = shaderStream.str().c_str();
            return shaderSource;
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

    }

// compiles and attaches a shader to the program
//-------------------------------------------------
    void attachShader(unsigned int shaderId, const char* shaderSource) {
        glShaderSource(shaderId, 1, &shaderSource, 0);
        glCompileShader(shaderId);
        glAttachShader(shaderId);
    }
};
#endif

