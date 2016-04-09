#pragma once

// Author(s): Tom
struct Texture
{
	GLuint id;
};

// Renderer functions

inline void Renderer_BindTexture(const Texture& texture)
{
	glBindTexture(GL_TEXTURE_2D, texture.id);
}

inline void Renderer_UnbindTexture(const Texture& texture)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer_ReadTexture(Texture* texture, const char* fileName)
{
	Image image = ReadImage(fileName);
	switch(image.format)
	{
	case IMAGE_FORMAT_BMP:
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height,
				0, GL_BGR, GL_UNSIGNED_BYTE, image.data);

		glGenerateMipmap(GL_TEXTURE_2D);
		FreeImage(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	}
}