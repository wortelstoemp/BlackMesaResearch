#pragma once

typedef unsigned char uchar;
typedef uint8_t uint8;
typedef int32_t int32;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef unsigned int uint;
typedef unsigned long ulong;

#define Assert(Expression) \
	if(!(Expression)) {*(uint32*)0 = 0;}
