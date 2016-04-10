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
	const int length = strlen(fileName);
	const char* extension = fileName + length - 4;

	if (!strcmp(extension, ".dds"))
	{
		DDSImage image;
		image.Create(fileName);
		// ...
		image.Delete();
	}
	else if (!strcmp(extension, ".bmp"))
	{
		BMPImage image;
		image.Create(fileName);
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height,
				0, GL_BGR, GL_UNSIGNED_BYTE, image.data);

		glGenerateMipmap(GL_TEXTURE_2D);
		image.Delete();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		printf("Texture file format not supported!\n");
	}
}