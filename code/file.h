#pragma once

// Author(s): Simon, Tom

// API Usage code:
// char* file = ReadFile("./path/file.txt");
// ...
// FreeFile(file);

char* ReadFile(const char* fileName)
{
	FILE* f = {0};
	fopen_s(&f, fileName, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		ulong fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* str = (char*) malloc(fsize + 1);
		fread(str, fsize, 1, f);
		fclose(f);
		str[fsize] = 0;
		
		return str;
	}
	
	return 0;
}

void FreeFile(char* file)
{
	free(file);
}
