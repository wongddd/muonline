#pragma once

#include <string>
#include <vector>

// Initialize OpenGL 2.0+ extension function pointers (loaded via wglGetProcAddress)
bool InitGLExtensions();

class CShader
{
public:
    CShader();
    virtual ~CShader();

    bool LoadVertexFromFile(const char* filename);
    bool LoadFragmentFromFile(const char* filename);
    bool LoadVertexFromString(const char* source);
    bool LoadFragmentFromString(const char* source);
    bool Link();
    void Use();
    void Unuse();

    unsigned int GetProgram() const { return m_Program; }
    bool IsLinked() const { return m_bLinked; }

    void SetUniform1i(const char* name, int value);
    void SetUniform1f(const char* name, float value);
    void SetUniform3fv(const char* name, const float* value);
    void SetUniform4fv(const char* name, const float* value);
    void SetUniformMatrix4fv(const char* name, const float* matrix);

private:
    unsigned int m_VertexShader;
    unsigned int m_FragmentShader;
    unsigned int m_Program;
    bool m_bLinked;

    unsigned int LoadShader(unsigned int type, const char* source);
    bool CheckCompileError(unsigned int shader);
    bool CheckLinkError(unsigned int program);
};

// Built-in shader management
bool InitBuiltinShaders();
void DestroyBuiltinShaders();

extern CShader* g_pTerrainShader;
extern CShader* g_pSimpleTextureShader;
