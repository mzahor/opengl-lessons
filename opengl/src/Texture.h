#pragma once
#include <string>
#include "glutils.h"

class Texture
{
private:
	uint m_RendererId;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BCC;

public:
	Texture(const std::string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};
