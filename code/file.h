#pragma once

// Author(s): Simon

#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char* fileName)
{
	FILE* f = fopen("textfile.txt", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* string = (char*)malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;
	return string;
}
