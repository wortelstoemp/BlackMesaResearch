#pragma once

// Author(s): Tom

// Usage:
// Image image = ReadImage("./file.dds");
// ...
// FreeImage(image);

enum ImageFormat
{
	IMAGE_FORMAT_BMP,
	IMAGE_FORMAT_DDS	
};

struct Image
{
	unsigned char* data;
	unsigned int width;
	unsigned int height;
	ImageFormat format;
};

Image ReadDDS(const char* fileName)
{
	printf("DDS\n");
	Image image = {0};
	image.format = IMAGE_FORMAT_DDS;
	return image;
}

// NOTE(Tom): Only used for testing, use .DDS files for performance
Image ReadBMP(const char* fileName)
{
	Image image;
	FILE* file;
	fopen_s(&file, fileName, "rb");
	if (!file)
	{
		printf("Can't open image!\n");
		image = {0};
		return image;
	}
	
	const int headerSize = 54;
	unsigned char header[headerSize];
	
	if (fread(header, 1, headerSize, file) != headerSize ||
		header[0] != 'B' || header[1] != 'M' ||
		*(int*)&(header[28]) != 24 || *(int*)&(header[30]) != 0)
	{
    	printf("Is not a correct 24 bit BMP image!\n");
		fclose(file);
		image = {0};
    	return image;	
	}
	
	image.width = *(int*)&(header[18]);
	image.height = *(int*)&(header[22]);
	image.format = IMAGE_FORMAT_BMP;
	unsigned int imageSize = *(int*)&(header[34]);
	
	if (imageSize == 0)
	{
		imageSize = image.width * image.height * 3; // RGB
	}
	
	image.data = (unsigned char*) malloc(sizeof(unsigned char) * imageSize);
	fread(image.data, 1, imageSize, file);
	fclose(file);
	
	return image;
}

Image ReadImage(const char* fileName)
{
	const int length = strlen(fileName);
	const char* extension = fileName + length - 4;
	Image image;
	
	if (!strcmp(extension, ".dds"))
	{
		image = ReadDDS(fileName);
	}
	else if (!strcmp(extension, ".bmp"))
	{
		image = ReadBMP(fileName);
	}
	else
	{
		image = {0};
		printf("Image file format not supported!\n");
	}
	
	return image;
}

void FreeImage(const Image& image)
{
	free(image.data);
}