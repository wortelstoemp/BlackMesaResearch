#pragma once

// Author(s): Tom

// API Usage code (same for BMPImage):
// DDSImage image;
// image.LoadFromFile(fileName);
// ...
// image.Delete();

// Compress PNG images with AMDCompress(CLI) to DDS.
// Only use compressed images (DXT1 = BC1), (DXT3 = BC2), (DXT5 = BC3).
// For sprites compress to DXT5 for alpha (gradient) channel.

const uint32 FOURCC_DXT1 = 0x31545844;
const uint32 FOURCC_DXT3 = 0x33545844;
const uint32 FOURCC_DXT5 = 0x35545844;
const uint32 COMPRESSED_RGBA_S3TC_DXT1_EXT = 0x83F1;
const uint32 COMPRESSED_RGBA_S3TC_DXT3_EXT = 0x83F2;
const uint32 COMPRESSED_RGBA_S3TC_DXT5_EXT = 0x83F3;

struct DDSImage
{
	uint8* data;
	int32 height;
	int32 width;
	int32 linearSize;
	int32 mipMapCount;
	int32 fourCC;
	uint8 blocksize;
	uint32 format;

	void LoadFromFile(const char* fileName)
	{
		FILE* file;
		fopen_s(&file, fileName, "rb");
		if (!file)
		{
			printf("Can't open image!\n");
			return;
		}

		const int32 headerSize = 124;
		uchar header[headerSize];

		char fileType[4];
		fread(fileType, 1, 4, file);
		if (strncmp(fileType, "DDS ", 4))
		{
			printf("Is not a correct DDS image!\n");
			fclose(file);
			return;
		}

		fread(&header, headerSize, 1, file);

		this->height = *(uint32*)&(header[8]);
		this->width	= *(uint32*)&(header[12]);
		this->linearSize = *(uint32*)&(header[16]);
		this->mipMapCount = *(uint32*)&(header[24]);
		this->fourCC = *(uint32*)&(header[80]);

		const uint32 imageSize = this->mipMapCount > 1 ? this->linearSize * 2 : this->linearSize;
		this->data = (unsigned char*) malloc(imageSize * sizeof(unsigned char));

		switch(this->fourCC)
		{
		case FOURCC_DXT1:
			this->format = COMPRESSED_RGBA_S3TC_DXT1_EXT;
			this->blocksize = 8;
			break;
		case FOURCC_DXT3:
			this->format = COMPRESSED_RGBA_S3TC_DXT3_EXT;
			this->blocksize = 16;
			break;
		case FOURCC_DXT5:
			this->format = COMPRESSED_RGBA_S3TC_DXT5_EXT;
			this->blocksize = 16;
			break;
		}

		if(this->data)
		{
			fread(this->data, 1, imageSize, file);
		}
		fclose(file);
	}

	inline void Delete()
	{
		if (this->data)
		{
			free(this->data);
		}
	}
};

// NOTE(Tom): BMP only used for testing, use DDS files for performance.
struct BMPImage
{
	unsigned char* data;
	uint32 width;
	uint32 height;

	void LoadFromFile(const char* fileName)
	{
		FILE* file;
		fopen_s(&file, fileName, "rb");
		if (!file)
		{
			printf("Can't open image!\n");
			return;
		}

		const int32 headerSize = 54;
		unsigned char header[headerSize];

		if (fread(header, 1, headerSize, file) != headerSize ||
			header[0] != 'B' || header[1] != 'M' ||
			*(int32*)&(header[28]) != 24 || *(int32*)&(header[30]) != 0)
		{

			printf("Is not a correct 24 bit BMP image!\n");
			fclose(file);
    		return;
		}

		this->width = *(int32*)&(header[18]);
		this->height = *(int32*)&(header[22]);
		uint32 imageSize = *(int32*)&(header[34]);

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
