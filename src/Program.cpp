#include "Program.h"
#include <iostream>
#include <vector>
#include "GLSL.h"

void Program::setShaderNames(const std::string &v, const std::string &f, const std::string &g) {
    vShaderName = v;
    fShaderName = f;
    gShaderName = g;
}

void Program::setShaderNames(const std::string &v, const std::string &f) {
    vShaderName = v;
    fShaderName = f;
}

GLuint Program::compileShader(GLenum shaderType, const std::string &shaderSourceFile) {
    GLint compileSuccess;
    
    // Read the shader source file into a string
    char *shaderString = GLSL::textFileRead(shaderSourceFile.c_str());
    // Stop if there was an error reading the shader source file
    if (shaderString == NULL) return 0;
    
    // Create the shader, assign source code, and compile it
    GLuint shader = glCreateShader(shaderType);
    CHECKED_GL_CALL(glShaderSource(shader, 1, &shaderString, NULL));
    CHECKED_GL_CALL(glCompileShader(shader));
    // See whether compile was successful
    CHECKED_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess));
    if (!compileSuccess) {
        if (isVerbose()) {
            GLSL::printShaderInfoLog(shader);
            std::cout << "Error compiling shader: " << shaderSourceFile << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    
    // Free the memory
    free(shaderString);
    
    return shader;
}

bool Program::init() {
    GLint linkSuccess;
    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vShaderName);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fShaderName);
    GLuint geomShader = compileShader(GL_GEOMETRY_SHADER, gShaderName);

    // Create the program and link it with the compiled shaders
    pid = glCreateProgram();
    if (vertShader > 0) CHECKED_GL_CALL(glAttachShader(pid, vertShader));
    if (fragShader > 0) CHECKED_GL_CALL(glAttachShader(pid, fragShader));
    if (geomShader > 0) CHECKED_GL_CALL(glAttachShader(pid, geomShader));
    CHECKED_GL_CALL(glLinkProgram(pid));
    // See whether link was successful
    CHECKED_GL_CALL(glGetProgramiv(pid, GL_LINK_STATUS, &linkSuccess));
    if (!linkSuccess) {
        if (isVerbose()) {
            GLSL::printProgramInfoLog(pid);
            std::cout << "Error linking shaders " << vShaderName << " and " << fShaderName;
            if (geomShader > 0) std::cout << " and " << gShaderName << std::endl;
            else std::cout << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    
    // Process the vertex and fragment shader programs to automatically add attribute and uniform variables
    findAttributesAndUniforms(vShaderName);
    findAttributesAndUniforms(fShaderName);

    return linkSuccess;
}

void Program::bind() {
    CHECKED_GL_CALL(glUseProgram(pid));
}

void Program::unbind() {
    CHECKED_GL_CALL(glUseProgram(0));
}

void Program::addAttribute(const std::string &name) {
    attributes[name] = GLSL::getAttribLocation(pid, name.c_str(), isVerbose());
}

void Program::addUniform(const std::string &name) {
    uniforms[name] = GLSL::getUniformLocation(pid, name.c_str(), isVerbose());
}

GLint Program::getAttribute(const std::string &name) const {
    std::map<std::string, GLint>::const_iterator attribute = attributes.find(name.c_str());
    if (attribute == attributes.end()) {
        if (isVerbose()) {
            std::cout << name << " is not an attribute variable in " << vShaderName << std::endl;
        }
        return -1;
    }
    return attribute->second;
}

GLint Program::getUniform(const std::string &name) const {
    std::map<std::string, GLint>::const_iterator uniform = uniforms.find(name.c_str());
    if (uniform == uniforms.end()) {
        if (isVerbose()) {
            std::cout << name << " is not a uniform variable in " << vShaderName << " or " << fShaderName << std::endl;
        }
        return -1;
    }
    return uniform->second;
}

// TODO: allow multiple variables on the same line with commas
void Program::findAttributesAndUniforms(const std::string &shaderSourceFile) {
    char *fileText = GLSL::textFileRead(shaderSourceFile.c_str());
    char *token;
    char *lastToken;
    
    std::vector<char *> lines;
    
    // Read the first line
    token = strtok(fileText, ";\n");
    lines.push_back(token);
    // Read all subsequent lines
    while((token = strtok(NULL, ";\n")) != NULL) {
        lines.push_back(token);
    }
//    std::cout << shaderSourceFile << std::endl;
//    std::cout << "Lines read: " << lines.size() << std::endl;
    
    // Look for keywords per line
    for (char *line : lines) {
        token = strtok(line, " (\n");
        if (token == NULL) continue;
        if (strcmp(token, "uniform") == 0) {
            
            // Handle lines with multiple variables separated by commas
            char *lineEnding = line + strlen(line) + 1;
            int lastDelimiter = -1;
            int lineEndingLength = strlen(lineEnding);
            for (int i = 0; i < lineEndingLength; i++) {
                if (lineEnding[i] == ',') {
                    lineEnding[i] = '\0';
                    addUniform(lineEnding + (lastDelimiter + 1));
                    lastDelimiter = i;
                } else if (lineEnding[i] == ' ' || lineEnding[i] == '\t') {
                    lastDelimiter = i;
                }
            }
            addUniform(lineEnding + (lastDelimiter + 1));
        } else if (strcmp(token, "layout") == 0) {
            while((token = strtok(NULL, " ")) != NULL) {
                lastToken = token;
            }
            addAttribute(lastToken);
        } else {
            continue;
        }
    }
    
    // Free the memory
    free(fileText);
}

void Program::setMatrix(const char *name, const GLfloat *value) {
    CHECKED_GL_CALL(glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value));
}
void Program::setMatrixArray(const char *name, const GLsizei count, const GLfloat *value) {
    CHECKED_GL_CALL(glUniformMatrix4fv(getUniform(name), count, GL_FALSE, value));
}
void Program::setVector2(const char *name, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform2fv(getUniform(name), 1, value));
}
void Program::setVector2(const char *name, const GLfloat x, const GLfloat y) {
    CHECKED_GL_CALL(glUniform2f(getUniform(name), x, y));
}
void Program::setVector2Array(const char *name, const GLsizei count, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform2fv(getUniform(name), count, value));
}
void Program::setVector3(const char *name, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform3fv(getUniform(name), 1, value));
}
void Program::setVector3(const char *name, const GLfloat x, const GLfloat y, const GLfloat z) {
    CHECKED_GL_CALL(glUniform3f(getUniform(name), x, y, z));
}
void Program::setVector3Array(const char *name, const GLsizei count, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform3fv(getUniform(name), count, value));
}
void Program::setVector4(const char *name, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform4fv(getUniform(name), 1, value));
}
void Program::setVector4(const char *name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) {
    CHECKED_GL_CALL(glUniform4f(getUniform(name), x, y, z, w));
}
void Program::setVector4Array(const char *name, const GLsizei count, const GLfloat *value) {
    CHECKED_GL_CALL(glUniform4fv(getUniform(name), count, value));
}
void Program::setFloat(const char *name, const GLfloat value) {
    CHECKED_GL_CALL(glUniform1f(getUniform(name), value));
}
void Program::setInt(const char *name, const GLint value) {
    CHECKED_GL_CALL(glUniform1i(getUniform(name), value));
}
void Program::setMVP(const GLfloat *M, const GLfloat *V, const GLfloat *P) {
    setMatrix("M", M);
    setMatrix("V", V);
    setMatrix("P", P);
}
