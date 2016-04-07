#pragma once

// Author(s): Simon

#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char* fileName)
{
	FILE* f = {0};
	fopen_s(&f, fileName, "rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		unsigned long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* string = (char*)malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;
		return string;
	}
	else
	{
		return 0;
	}
}
