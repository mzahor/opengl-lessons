#pragma once

#include <GL/glew.h>

#define ASSERT(X) \
	if ((X))      \
	__debugbreak()

#define GLCall(X)    \
	GLCleanErrors(); \
	X;               \
	ASSERT(GLPrintErrors(#X, __FILE__, __LINE__))

using uint = unsigned int;


void GLCleanErrors();
GLenum GLPrintErrors(const char *function, const char *file, long line);
