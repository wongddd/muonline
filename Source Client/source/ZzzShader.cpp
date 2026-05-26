#include "stdafx.h"
#include "ZzzShader.h"
#include "Utilities\Log\ErrorReport.h"

// ============================================================
// OpenGL extension function pointers (loaded via wglGetProcAddress)
// ============================================================

// Use void* for function pointers to avoid conflicts with GLEW types
static void* s_glCreateShader = NULL;
static void* s_glDeleteShader = NULL;
static void* s_glShaderSource = NULL;
static void* s_glCompileShader = NULL;
static void* s_glGetShaderiv = NULL;
static void* s_glGetShaderInfoLog = NULL;
static void* s_glCreateProgram = NULL;
static void* s_glDeleteProgram = NULL;
static void* s_glAttachShader = NULL;
static void* s_glLinkProgram = NULL;
static void* s_glGetProgramiv = NULL;
static void* s_glGetProgramInfoLog = NULL;
static void* s_glUseProgram = NULL;
static void* s_glGetUniformLocation = NULL;
static void* s_glUniform1i = NULL;
static void* s_glUniform1f = NULL;
static void* s_glUniform3fv = NULL;
static void* s_glUniform4fv = NULL;
static void* s_glUniformMatrix4fv = NULL;

// Safe call macros
#define GL_CALL(func, args) \
    do { \
        typedef void (__stdcall* FuncType) args; \
        FuncType f = (FuncType)s_##func; \
        if (f) f; \
    } while(0)

#define GL_CALL_RET(func, args, ret) \
    do { \
        typedef void (__stdcall* FuncType) args; \
        FuncType f = (FuncType)s_##func; \
        if (f) { f; } \
    } while(0)

#define GL_CALL_UINT(func, args) \
    ({ \
        typedef unsigned int (__stdcall* FuncType) args; \
        FuncType f = (FuncType)s_##func; \
        f ? f() : 0; \
    })

// Specific function wrappers
static unsigned int __stdcall w_glCreateShader(unsigned int type)
{
    typedef unsigned int (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glCreateShader;
    return f ? f(type) : 0;
}

static void __stdcall w_glDeleteShader(unsigned int shader)
{
    typedef void (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glDeleteShader;
    if (f) f(shader);
}

static void __stdcall w_glShaderSource(unsigned int shader, int count, const char** str, const int* length)
{
    typedef void (__stdcall* PF)(unsigned int, int, const char**, const int*);
    PF f = (PF)s_glShaderSource;
    if (f) f(shader, count, str, length);
}

static void __stdcall w_glCompileShader(unsigned int shader)
{
    typedef void (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glCompileShader;
    if (f) f(shader);
}

static void __stdcall w_glGetShaderiv(unsigned int shader, unsigned int pname, int* params)
{
    typedef void (__stdcall* PF)(unsigned int, unsigned int, int*);
    PF f = (PF)s_glGetShaderiv;
    if (f) f(shader, pname, params);
}

static void __stdcall w_glGetShaderInfoLog(unsigned int shader, int bufSize, int* length, char* infoLog)
{
    typedef void (__stdcall* PF)(unsigned int, int, int*, char*);
    PF f = (PF)s_glGetShaderInfoLog;
    if (f) f(shader, bufSize, length, infoLog);
}

static unsigned int __stdcall w_glCreateProgram()
{
    typedef unsigned int (__stdcall* PF)();
    PF f = (PF)s_glCreateProgram;
    return f ? f() : 0;
}

static void __stdcall w_glDeleteProgram(unsigned int program)
{
    typedef void (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glDeleteProgram;
    if (f) f(program);
}

static void __stdcall w_glAttachShader(unsigned int program, unsigned int shader)
{
    typedef void (__stdcall* PF)(unsigned int, unsigned int);
    PF f = (PF)s_glAttachShader;
    if (f) f(program, shader);
}

static void __stdcall w_glLinkProgram(unsigned int program)
{
    typedef void (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glLinkProgram;
    if (f) f(program);
}

static void __stdcall w_glGetProgramiv(unsigned int program, unsigned int pname, int* params)
{
    typedef void (__stdcall* PF)(unsigned int, unsigned int, int*);
    PF f = (PF)s_glGetProgramiv;
    if (f) f(program, pname, params);
}

static void __stdcall w_glGetProgramInfoLog(unsigned int program, int bufSize, int* length, char* infoLog)
{
    typedef void (__stdcall* PF)(unsigned int, int, int*, char*);
    PF f = (PF)s_glGetProgramInfoLog;
    if (f) f(program, bufSize, length, infoLog);
}

static void __stdcall w_glUseProgram(unsigned int program)
{
    typedef void (__stdcall* PF)(unsigned int);
    PF f = (PF)s_glUseProgram;
    if (f) f(program);
}

static int __stdcall w_glGetUniformLocation(unsigned int program, const char* name)
{
    typedef int (__stdcall* PF)(unsigned int, const char*);
    PF f = (PF)s_glGetUniformLocation;
    return f ? f(program, name) : -1;
}

static void __stdcall w_glUniform1i(int location, int v0)
{
    typedef void (__stdcall* PF)(int, int);
    PF f = (PF)s_glUniform1i;
    if (f) f(location, v0);
}

static void __stdcall w_glUniform1f(int location, float v0)
{
    typedef void (__stdcall* PF)(int, float);
    PF f = (PF)s_glUniform1f;
    if (f) f(location, v0);
}

static void __stdcall w_glUniform3fv(int location, int count, const float* value)
{
    typedef void (__stdcall* PF)(int, int, const float*);
    PF f = (PF)s_glUniform3fv;
    if (f) f(location, count, value);
}

static void __stdcall w_glUniform4fv(int location, int count, const float* value)
{
    typedef void (__stdcall* PF)(int, int, const float*);
    PF f = (PF)s_glUniform4fv;
    if (f) f(location, count, value);
}

static void __stdcall w_glUniformMatrix4fv(int location, int count, unsigned char transpose, const float* value)
{
    typedef void (__stdcall* PF)(int, int, unsigned char, const float*);
    PF f = (PF)s_glUniformMatrix4fv;
    if (f) f(location, count, transpose, value);
}

bool InitGLExtensions()
{
    s_glCreateShader = (void*)wglGetProcAddress("glCreateShader");
    s_glDeleteShader = (void*)wglGetProcAddress("glDeleteShader");
    s_glShaderSource = (void*)wglGetProcAddress("glShaderSource");
    s_glCompileShader = (void*)wglGetProcAddress("glCompileShader");
    s_glGetShaderiv = (void*)wglGetProcAddress("glGetShaderiv");
    s_glGetShaderInfoLog = (void*)wglGetProcAddress("glGetShaderInfoLog");
    s_glCreateProgram = (void*)wglGetProcAddress("glCreateProgram");
    s_glDeleteProgram = (void*)wglGetProcAddress("glDeleteProgram");
    s_glAttachShader = (void*)wglGetProcAddress("glAttachShader");
    s_glLinkProgram = (void*)wglGetProcAddress("glLinkProgram");
    s_glGetProgramiv = (void*)wglGetProcAddress("glGetProgramiv");
    s_glGetProgramInfoLog = (void*)wglGetProcAddress("glGetProgramInfoLog");
    s_glUseProgram = (void*)wglGetProcAddress("glUseProgram");
    s_glGetUniformLocation = (void*)wglGetProcAddress("glGetUniformLocation");
    s_glUniform1i = (void*)wglGetProcAddress("glUniform1i");
    s_glUniform1f = (void*)wglGetProcAddress("glUniform1f");
    s_glUniform3fv = (void*)wglGetProcAddress("glUniform3fv");
    s_glUniform4fv = (void*)wglGetProcAddress("glUniform4fv");
    s_glUniformMatrix4fv = (void*)wglGetProcAddress("glUniformMatrix4fv");

    if (!s_glCreateShader || !s_glCreateProgram || !s_glUseProgram)
    {
        g_ErrorReport.Write("[GL Ext] GLSL functions not available on this GPU/driver.\r\n");
        return false;
    }
    g_ErrorReport.Write("[GL Ext] OpenGL 2.0+ shader functions loaded successfully.\r\n");
    return true;
}

// ============================================================
// Built-in shader sources (GLSL v120 for OpenGL 2.1+ compat)
// ============================================================

static const char* g_szTerrainVertex =
    "varying vec2 v_TexCoord;\n"
    "varying vec4 v_Color;\n"
    "void main()\n"
    "{\n"
    "    v_TexCoord = gl_MultiTexCoord0.xy;\n"
    "    v_Color = gl_Color;\n"
    "    gl_Position = ftransform();\n"
    "    gl_FogFragCoord = gl_Position.z;\n"
    "}\n";

static const char* g_szTerrainFragment =
    "varying vec2 v_TexCoord;\n"
    "varying vec4 v_Color;\n"
    "uniform sampler2D u_Texture;\n"
    "void main()\n"
    "{\n"
    "    vec4 texColor = texture2D(u_Texture, v_TexCoord);\n"
    "    gl_FragColor = texColor * v_Color;\n"
    "}\n";

static const char* g_szSimpleTextureVertex =
    "varying vec2 v_TexCoord;\n"
    "varying vec4 v_Color;\n"
    "void main()\n"
    "{\n"
    "    v_TexCoord = gl_MultiTexCoord0.xy;\n"
    "    v_Color = gl_Color;\n"
    "    gl_Position = ftransform();\n"
    "}\n";

static const char* g_szSimpleTextureFragment =
    "varying vec2 v_TexCoord;\n"
    "varying vec4 v_Color;\n"
    "uniform sampler2D u_Texture;\n"
    "void main()\n"
    "{\n"
    "    vec4 texColor = texture2D(u_Texture, v_TexCoord);\n"
    "    gl_FragColor = texColor * v_Color;\n"
    "}\n";

// ============================================================
// CShader implementation
// ============================================================

CShader::CShader()
    : m_VertexShader(0)
    , m_FragmentShader(0)
    , m_Program(0)
    , m_bLinked(false)
{
}

CShader::~CShader()
{
    if (m_Program)
    {
        w_glDeleteProgram(m_Program);
        m_Program = 0;
    }
    if (m_VertexShader)
    {
        w_glDeleteShader(m_VertexShader);
        m_VertexShader = 0;
    }
    if (m_FragmentShader)
    {
        w_glDeleteShader(m_FragmentShader);
        m_FragmentShader = 0;
    }
}

unsigned int CShader::LoadShader(unsigned int type, const char* source)
{
    unsigned int shader = w_glCreateShader(type);
    if (shader == 0)
        return 0;

    w_glShaderSource(shader, 1, &source, NULL);
    w_glCompileShader(shader);

    if (!CheckCompileError(shader))
    {
        w_glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool CShader::CheckCompileError(unsigned int shader)
{
    int compiled = 0;
    w_glGetShaderiv(shader, 0x8B81 /* GL_COMPILE_STATUS */, &compiled);
    if (compiled == 0)
    {
        char log[1024] = { 0 };
        int len = 0;
        w_glGetShaderInfoLog(shader, sizeof(log) - 1, &len, log);
        g_ErrorReport.Write("[CShader] Compile error: %s\r\n", log);
        return false;
    }
    return true;
}

bool CShader::CheckLinkError(unsigned int program)
{
    int linked = 0;
    w_glGetProgramiv(program, 0x8B82 /* GL_LINK_STATUS */, &linked);
    if (linked == 0)
    {
        char log[1024] = { 0 };
        int len = 0;
        w_glGetProgramInfoLog(program, sizeof(log) - 1, &len, log);
        g_ErrorReport.Write("[CShader] Link error: %s\r\n", log);
        return false;
    }
    return true;
}

bool CShader::LoadVertexFromString(const char* source)
{
    m_VertexShader = LoadShader(0x8B31 /* GL_VERTEX_SHADER */, source);
    return (m_VertexShader != 0);
}

bool CShader::LoadFragmentFromString(const char* source)
{
    m_FragmentShader = LoadShader(0x8B30 /* GL_FRAGMENT_SHADER */, source);
    return (m_FragmentShader != 0);
}

bool CShader::LoadVertexFromFile(const char* filename) { return false; }
bool CShader::LoadFragmentFromFile(const char* filename) { return false; }

bool CShader::Link()
{
    if (m_VertexShader == 0 || m_FragmentShader == 0)
        return false;

    m_Program = w_glCreateProgram();
    if (m_Program == 0)
        return false;

    w_glAttachShader(m_Program, m_VertexShader);
    w_glAttachShader(m_Program, m_FragmentShader);
    w_glLinkProgram(m_Program);

    m_bLinked = CheckLinkError(m_Program);
    return m_bLinked;
}

void CShader::Use()
{
    if (m_bLinked)
        w_glUseProgram(m_Program);
}

void CShader::Unuse()
{
    w_glUseProgram(0);
}

void CShader::SetUniform1i(const char* name, int value)
{
    if (!m_bLinked) return;
    int loc = w_glGetUniformLocation(m_Program, name);
    if (loc >= 0) w_glUniform1i(loc, value);
}

void CShader::SetUniform1f(const char* name, float value)
{
    if (!m_bLinked) return;
    int loc = w_glGetUniformLocation(m_Program, name);
    if (loc >= 0) w_glUniform1f(loc, value);
}

void CShader::SetUniform3fv(const char* name, const float* value)
{
    if (!m_bLinked) return;
    int loc = w_glGetUniformLocation(m_Program, name);
    if (loc >= 0) w_glUniform3fv(loc, 1, value);
}

void CShader::SetUniform4fv(const char* name, const float* value)
{
    if (!m_bLinked) return;
    int loc = w_glGetUniformLocation(m_Program, name);
    if (loc >= 0) w_glUniform4fv(loc, 1, value);
}

void CShader::SetUniformMatrix4fv(const char* name, const float* matrix)
{
    if (!m_bLinked) return;
    int loc = w_glGetUniformLocation(m_Program, name);
    if (loc >= 0) w_glUniformMatrix4fv(loc, 1, 0, matrix);
}

// ============================================================
// Built-in shader management
// ============================================================

CShader* g_pTerrainShader = NULL;
CShader* g_pSimpleTextureShader = NULL;

bool InitBuiltinShaders()
{
    if (!InitGLExtensions())
    {
        g_ErrorReport.Write("[Shader] GLSL not available, using fixed-function only.\r\n");
        return false;
    }

    g_pTerrainShader = new CShader();
    if (!g_pTerrainShader->LoadVertexFromString(g_szTerrainVertex))
    {
        g_ErrorReport.Write("[Shader] Failed to load terrain vertex shader.\r\n");
        return false;
    }
    if (!g_pTerrainShader->LoadFragmentFromString(g_szTerrainFragment))
    {
        g_ErrorReport.Write("[Shader] Failed to load terrain fragment shader.\r\n");
        return false;
    }
    if (!g_pTerrainShader->Link())
    {
        g_ErrorReport.Write("[Shader] Failed to link terrain shader program.\r\n");
        return false;
    }
    g_pTerrainShader->Use();
    g_pTerrainShader->SetUniform1i("u_Texture", 0);
    g_pTerrainShader->Unuse();

    g_pSimpleTextureShader = new CShader();
    if (!g_pSimpleTextureShader->LoadVertexFromString(g_szSimpleTextureVertex))
    {
        g_ErrorReport.Write("[Shader] Failed to load simple texture vertex shader.\r\n");
        return false;
    }
    if (!g_pSimpleTextureShader->LoadFragmentFromString(g_szSimpleTextureFragment))
    {
        g_ErrorReport.Write("[Shader] Failed to load simple texture fragment shader.\r\n");
        return false;
    }
    if (!g_pSimpleTextureShader->Link())
    {
        g_ErrorReport.Write("[Shader] Failed to link simple texture shader program.\r\n");
        return false;
    }

    g_pSimpleTextureShader->Use();
    g_pSimpleTextureShader->SetUniform1i("u_Texture", 0);
    g_pSimpleTextureShader->Unuse();

    g_ErrorReport.Write("> Built-in shaders initialized successfully.\r\n");
    return true;
}

void DestroyBuiltinShaders()
{
    if (g_pTerrainShader)
    {
        delete g_pTerrainShader;
        g_pTerrainShader = NULL;
    }
    if (g_pSimpleTextureShader)
    {
        delete g_pSimpleTextureShader;
        g_pSimpleTextureShader = NULL;
    }
}
