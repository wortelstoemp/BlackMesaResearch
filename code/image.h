#pragma once

// Author(s): Tom

// Usage:
// DDSImage image;
// image.Create(fileName);
// ...
// image.Delete();

// Compress PNG images with AMDCompress(CLI) to DDS.
// Only use compressed images (DXT1 = BC1), (DXT3 = BC2), (DXT5 = BC3).
// For sprites compress to DXT5 for alpha (gradient) channel.
struct DDSImage
{
	unsigned char* data;
	unsigned int height;
	unsigned int width;
	unsigned int linearSize;
	unsigned int mipMapCount;
	unsigned int fourCC;
	
	void Create(const char* fileName)
	{
		FILE* file; 
		fopen_s(&file, fileName, "rb"); 
		if (!file)
		{
			printf("Can't open image!\n");
			return;
		}
		
		const int headerSize = 124;
		unsigned char header[headerSize];
		
		char fileType[4]; 
		fread(fileType, 1, 4, file); 
		if (strncmp(fileType, "DDS ", 4))
		{
			printf("Is not a correct DDS image!\n");
			fclose(file); 
			return;
		}
		
		fread(&header, headerSize, 1, file); 

		this->height = *(unsigned int*)&(header[8]);
		this->width	= *(unsigned int*)&(header[12]);
		this->linearSize = *(unsigned int*)&(header[16]);
		this->mipMapCount = *(unsigned int*)&(header[24]);
		this->fourCC = *(unsigned int*)&(header[80]);
		
		const unsigned int imageSize = this->mipMapCount > 1 ? this->linearSize * 2 : this->linearSize;
		this->data = (unsigned char*) malloc(imageSize * sizeof(unsigned char));
		
		if(this->data)
		{
			fread(this->data, 1, imageSize, file); 
		}
		fclose(file);
	}
	
	inline void Delete()
	{
		free(this->data);
	}
};

// NOTE(Tom): BMP only used for testing, use DDS files for performance.
struct BMPImage
{
	unsigned char* data;
	unsigned int width;
	unsigned int height;
	
	void Create(const char* fileName)
	{
		FILE* file;
		fopen_s(&file, fileName, "rb");
		if (!file)
		{
			printf("Can't open image!\n");
			return;
		}
	
		const int headerSize = 54;
		unsigned char header[headerSize];
	
		if (fread(header, 1, headerSize, file) != headerSize ||
			header[0] != 'B' || header[1] != 'M' ||
			*(int*)&(header[28]) != 24 || *(int*)&(header[30]) != 0)
		{
    		printf("Is not a correct 24 bit BMP image!\n");
			fclose(file);
    		return;	
		}
	
		this->width = *(int*)&(header[18]);
		this->height = *(int*)&(header[22]);
		unsigned int imageSize = *(int*)&(header[34]);
	
		if (imageSize == 0)
		{
			imageSize = this->width * this->height * 3; // RGB
		}
	
		this->data = (unsigned char*) malloc(sizeof(unsigned char) * imageSize);
		if(this->data)
		{
			fread(this->data, 1, imageSize, file);
		}
		fclose(file);
	}
	
	inline void Delete()
	{
		free(this->data);
	}
};