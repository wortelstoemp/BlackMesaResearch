#pragma once

// Author(s): Tom

// Usage:
// DDSImage image;
// image.Create(fileName);
// ...
// image.Delete();

struct DDSImage
{
	unsigned char* data;
	unsigned int width;
	unsigned int height;
	
	void Create(const char* fileName)
	{
		printf("DDS\n");
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
		fread(this->data, 1, imageSize, file);
		fclose(file);
	}
	
	inline void Delete()
	{
		free(this->data);
	}
};