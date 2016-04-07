#pragma once

namespace image
{
	static unsigned char* LoadBMP(const char* fileName)
	{
		printf("BMP\n");
		return nullptr;
	}
	
	static unsigned char* LoadDDS(const char* fileName)
	{
		printf("DDS\n");
		return nullptr;
	}
	
	unsigned char* Load(const char* fileName)
	{
		const int length = strlen(fileName);
		if (!strcmp(fileName + length - 4, ".dds"))
		{
			return LoadDDS(fileName);
		}
		else if (!strcmp(fileName + length - 4, ".bmp"))
		{
			return LoadBMP(fileName);
		}
		
		printf("Image file format not supported!\n");
		return nullptr;
	}
} 