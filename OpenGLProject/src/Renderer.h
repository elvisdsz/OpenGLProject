#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"

#if defined(_MSC_VER)
    #include <intrin.h>
    #define debugbreak() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <signal.h>
    #define debugbreak() raise(SIGTRAP)
#else
    #error "Platform not supported"
#endif

#define ASSERT(x) if (!(x)) debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

class VertexArray; // forward declaration to avoid circular dependency

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};