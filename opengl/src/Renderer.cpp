#include <iostream>
#include "Renderer.h"

void GLCleanErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

GLenum GLPrintErrors(const char *function, const char *file, long line)
{
	while (const GLenum err = glGetError())
	{
		std::cout << "[OpenGL Error]: " << err << " " << function << " "
				  << " at " << file << ":" << line << std::endl;
		return err;
	}
	return 0;
}
