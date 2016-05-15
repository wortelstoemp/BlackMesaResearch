#pragma once

typedef unsigned char uchar;
typedef uint8_t uint8;
typedef int32_t int32;
typedef uint32_t uint32;
typedef unsigned int uint;
typedef unsigned long ulong;

#define Assert(Expression) \
	if(!(Expression)) {*(int*)0 = 0;}
