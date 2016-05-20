#pragma once

// Author(s): Tom

// API Usage code:
// Texture texture;
// texture.LoadFromFile("../data/textures/foo.bmp");
// texture.Use();
// ...
// texture.Unuse();

// Use MultiTexture in shader:
// Texture uniforms should follow the naming convention below.
// uniform sampler2D diffuseTexture1;
// uniform sampler2D diffuseTexture2;
// uniform sampler2D diffuseTexture3;
// ...
// uniform sampler2D specularTexture1;
// uniform sampler2D specularTexture2;
// uniform sampler2D specularTexture3;
// ...
// then use mix() function to mix those textures.



struct Texture
{
	enum TextureType
	{
		DIFFUSE,
		SPECULAR
	};

	GLuint id;
	TextureType type;

	bool LoadFromFile(const char* fileName)
	{
		const int32 length = strlen(fileName);
		const char* extension = fileName + length - 4;

		if (strcmp(extension, ".dds") == 0)
		{
			DDSImage image;
			image.LoadFromFile(fileName);

			uint32 format;
			uint32 blockSize;

			glGenTextures(1, &this->id);
			glBindTexture(GL_TEXTURE_2D, this->id);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			uint32 level = 0;
			uint32 offset = 0;
			while (level < image.mipMapCount && image.width > 0 && image.height > 0)
			{
				uint32 size = ((image.width + 3) / 4) * ((image.height + 3) / 4)	* image.blocksize;
				glCompressedTexImage2D(GL_TEXTURE_2D, level, image.format,
					image.width, image.height, 0, size, image.data + offset);

				offset += size;
				image.width /= 2;
				image.height /= 2;
				level++;
			}
			image.Delete();
			glBindTexture(GL_TEXTURE_2D, 0);

			return true;
		}

		if (strcmp(extension, ".bmp") == 0)
		{
			BMPImage image;
			image.LoadFromFile(fileName);
			glGenTextures(1, &this->id);
			glBindTexture(GL_TEXTURE_2D, this->id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height,
					0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
			glGenerateMipmap(GL_TEXTURE_2D);
			image.Delete();
			glBindTexture(GL_TEXTURE_2D, 0);

			return true;
		}

		printf("Texture file format not supported!\n");
		return false;
	}

	inline void Use()
	{
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	inline void Unuse()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

class MultiTexture
{
private:
	std::vector<Texture> textures;

public:
	inline void LoadTextureFromFile(const char* fileName, Texture::TextureType type)
	{
		Texture texture;
		texture.type = type;
		if (texture.LoadFromFile(fileName))
		{
			this->textures.push_back(texture);
		}
	}

	inline void AddTexture(const Texture& texture)
	{
		this->textures.push_back(texture);
	}

	inline void RemoveLastTexture()
	{
		this->textures.pop_back();
	}

	inline void ClearTextures()
	{
		this->textures.clear();
	}

	inline void Use(const Shader& shader)
	{
		int32 diffuseNr = 1;
		int32 specularNr = 1;

		const int32 numTextures = this->textures.size();
		for(GLuint i = 0; i < numTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			const Texture::TextureType type = this->textures[i].type;
			if (type == Texture::DIFFUSE)
			{
				char number[10];
				itoa(diffuseNr++, number, 10);
				char uniformName[15] = "textureDiffuse";
				strncat(uniformName, number, strlen(number));
				GLint uniform = glGetUniformLocation(shader.program, uniformName);
				//printf("%s\n", uniformName);
				glUniform1i(uniform, i);
			}
			else if (type == Texture::SPECULAR)
			{
				char number[10];
				itoa(specularNr++, number, 10);
				char uniformName[16] = "textureSpecular";
				strncat(uniformName, number, strlen(number));
				GLint uniform = glGetUniformLocation(shader.program, uniformName);
				glUniform1i(uniform, i);
			}
			else
			{
				break;
			}

            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
		glActiveTexture(GL_TEXTURE0);
	}

	inline void Unuse()
	{
		const int32 numTextures = this->textures.size();
		for(GLuint i = 0; i < numTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
	}
};
