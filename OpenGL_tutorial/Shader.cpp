#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    vertexId = 0;
    fragmentId = 0;
    tessControlId = 0;
    tessEvalId = 0;
    geometryId = 0;
    computeId = 0;
    programId = glCreateProgram();
    addShader(vertexPath, GL_VERTEX_SHADER);
    addShader(fragmentPath, GL_FRAGMENT_SHADER);
}

void Shader::addShader(const char* shaderFilePath, GLenum shaderType) {
    std::string shaderSource;
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(shaderFilePath, std::ifstream::in);
        shaderStream << shaderFile.rdbuf();
        shaderSource = shaderStream.str();
        shaderFile.close();
        attachShader(shaderType, shaderSource.c_str());
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

void Shader::linkAndCompile() {
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

void Shader::checkForCompileErrors(unsigned int shader, std::string type)
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

void Shader::attachShader(GLenum shaderType, const char* shaderSource) {

    switch (shaderType) {
    case GL_VERTEX_SHADER:
        vertexId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexId, 1, &shaderSource, 0);
        glCompileShader(vertexId);
        glAttachShader(programId, vertexId);
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