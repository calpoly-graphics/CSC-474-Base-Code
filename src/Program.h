#pragma  once
#ifndef LAB471_PROGRAM_H_INCLUDED
#define LAB471_PROGRAM_H_INCLUDED

#include <map>
#include <string>
#include <glad/glad.h>

class Program {
public:
	virtual bool init();
	virtual void bind();
	virtual void unbind();
    void setShaderNames(const std::string &v, const std::string &f, const std::string &g);
    void setShaderNames(const std::string &v, const std::string &f);
    void setVerbose(const bool v) { verbose = v; }
    bool isVerbose() const { return verbose; }
    
    void addAttribute(const std::string &name);
    void addUniform(const std::string &name);
    
    GLint getAttribute(const std::string &name) const;
    GLint getUniform(const std::string &name) const;
    GLuint getPID() { return pid; }
    
    void setMatrix(const char *name, const GLfloat *value);
    void setMatrixArray(const char *name, const GLsizei count, const GLfloat *value);
    void setVector2(const char *name, const GLfloat *value);
    void setVector2(const char *name, const GLfloat x, const GLfloat y);
    void setVector2Array(const char *name, const GLsizei count, const GLfloat *value);
    void setVector3(const char *name, const GLfloat *value);
    void setVector3(const char *name, const GLfloat x, const GLfloat y, const GLfloat z);
    void setVector3Array(const char *name, const GLsizei count, const GLfloat *value);
    void setVector4(const char *name, const GLfloat *value);
    void setVector4(const char *name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
    void setVector4Array(const char *name, const GLsizei count, const GLfloat *value);
    void setFloat(const char *name, const GLfloat value);
    void setInt(const char *name, const GLint value);
    
    void setMVP(const GLfloat *M, const GLfloat *V, const GLfloat *P);
	
protected:
    std::string vShaderName = ""; // Vertex shader
    std::string gShaderName = ""; // Geometry shader
    std::string fShaderName = ""; // Fragment shader

private:
    GLuint compileShader(GLenum shaderType, const std::string &shaderSourceFile);
    void findAttributesAndUniforms(const std::string &shaderSourceFile);
    
    GLuint pid = 0;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;
	bool verbose = true;
};

#endif // LAB471_PROGRAM_H_INCLUDED
