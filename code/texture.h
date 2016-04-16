#pragma once

// Author(s): Tom

// API Usage code:
// Texture texture;
// texture.CreateFromFile("../data/textures/foo.bmp");
// texture.Bind();
// ...
// texture.Unbind();

struct Texture
{
	GLuint id;

	void CreateFromFile(const char* fileName)
	{
		const int length = strlen(fileName);
		const char* extension = fileName + length - 4;

		if (!strcmp(extension, ".dds"))
		{
			DDSImage image;
			image.CreateFromFile(fileName);
			
			const unsigned int FOURCC_DXT1 = 0x31545844;
			const unsigned int FOURCC_DXT3 = 0x33545844;
			const unsigned int FOURCC_DXT5 = 0x35545844;
			const unsigned int COMPRESSED_RGBA_S3TC_DXT1_EXT = 0x83F1;
			const unsigned int COMPRESSED_RGBA_S3TC_DXT3_EXT = 0x83F2;
			const unsigned int COMPRESSED_RGBA_S3TC_DXT5_EXT = 0x83F3;
			unsigned int format;
			unsigned int blockSize;
			
			switch(image.fourCC) 
			{ 
			case FOURCC_DXT1: 
				format = COMPRESSED_RGBA_S3TC_DXT1_EXT; 
				blockSize = 8;
				break; 
			case FOURCC_DXT3: 
				format = COMPRESSED_RGBA_S3TC_DXT3_EXT;
				blockSize = 16;
				break; 
			case FOURCC_DXT5: 
				format = COMPRESSED_RGBA_S3TC_DXT5_EXT;
				blockSize = 16;
				break; 
			default: 
				image.Delete(); 
				return; 
			}
			
			glGenTextures(1, &this->id);
			glBindTexture(GL_TEXTURE_2D, this->id);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			unsigned int level = 0;
			unsigned int offset = 0;
			while (level < image.mipMapCount && image.width > 0 && image.height > 0)
			{ 
				unsigned int size = ((image.width+3)/4)*((image.height+3)/4)*blockSize; 
				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, image.width, image.height,  
					0, size, image.data + offset); 
	 
				offset += size; 
				image.width /= 2; 
				image.height /= 2;
				level++;
			} 
			image.Delete();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (!strcmp(extension, ".bmp"))
		{
			BMPImage image;
			image.CreateFromFile(fileName);
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
		}
		else
		{
			printf("Texture file format not supported!\n");
		}
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

struct MultiTexture
{
	std::vector<Texture> textures;
	
	inline void Use()
	{
		int numTextures = this->textures.size();
		for(GLuint i = 0; i < numTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
	}
	
	inline void Unuse()
	{
		int numTextures = this->textures.size();
		for(GLuint i = 0; i < numTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
	}
};