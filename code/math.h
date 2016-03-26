#pragma once

#include <cmath>

#define PI 3.14159265358979323846

// Author(s): Simon, Tom

/*************
 * Datatypes *
 *************/
 
struct Vec2
{
	float X;
	float Y;
};

union Vec3
{
	struct
	{
		Vec2 XY;
		float Z;
	};
	struct
	{
		float X;
		float Y;
		float Z;
	};
};

union Matrix2x2
{
	float values[4];
	struct
	{
		float a11;
		float a12;
		float a21;
		float a22;
	};
};

union Matrix3x3
{
	float values[9];
	struct
	{
		float a11;
		float a12;
		float a13;
		float a21;
		float a22;
		float a23;
		float a31;
		float a32;
		float a33;
	};
};

union Matrix4x4
{
	float values[16];
	struct
	{
		float a11;
		float a12;
		float a13;
		float a14;
		float a21;
		float a22;
		float a23;
		float a24;
		float a31;
		float a32;
		float a33;
		float a34;
		float a41;
		float a42;
		float a43;
		float a44;	
	};
};

struct Quaternion
{
	float X;
	float Y;
	float Z;
	float W;
};

/**************************
 * Operations & Functions *
 **************************/

// Vec2

inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
	Vec2 result = { lhs.X + rhs.X, lhs.Y + rhs.Y };
	return result;
}

inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
	Vec2 result = { lhs.X - rhs.X, lhs.Y - rhs.Y };
	return result;
}

inline Vec2 operator*(const Matrix2x2& lhs, const Vec2& rhs)
{
	Vec2 result = { 0 };
	result.X = lhs.a11 * rhs.X + lhs.a12 * rhs.Y;
	result.Y = lhs.a21 * rhs.X + lhs.a22 * rhs.Y;
	return result;
}

// Vec3
inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
	Vec3 result = { lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z};
	return result;
}

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
	Vec3 result = { lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z};
	return result;
}

inline Vec3 operator*(const Matrix3x3& lhs, const Vec3& rhs)
{
	Vec3 result = { 0 };

	result.X = lhs.a11 * rhs.X + lhs.a12 * rhs.Y + lhs.a13 * rhs.Z;
	result.Y = lhs.a21 * rhs.X + lhs.a22 * rhs.Y + lhs.a23 * rhs.Z;
	result.Z = lhs.a31 * rhs.X + lhs.a32 * rhs.Y + lhs.a33 * rhs.Z;
	
	return result;
}

inline Vec3 operator*(const Matrix4x4 lhs, const Vec3& rhs)
{
	Vec3 result = { 0 };

	result.X = lhs.a11 * rhs.X + lhs.a12 * rhs.Y + lhs.a13 * rhs.Z + lhs.a14;
	result.Y = lhs.a21 * rhs.X + lhs.a22 * rhs.Y + lhs.a23 * rhs.Z + lhs.a24;
	result.Z = lhs.a31 * rhs.X + lhs.a32 * rhs.Y + lhs.a33 * rhs.Z + lhs.a34;

	return result;
}

inline Vec3 operator*(const Vec3& lhs, const float rhs)
{
	Vec3 result = { lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs };
	return result;
}

inline Vec3 operator*(const float lhs, const Vec3& rhs)
{
	return rhs * lhs;
}

inline Vec3 operator/(const Vec3& lhs, const float& rhs)
{
	Vec3 result = { lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs};
	return result;
}

inline float Dot(const Vec3& lhs, const Vec3& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

inline Vec3 Cross(const Vec3& lhs, const Vec3& rhs)
{
	Vec3 result =
	{ 
		(lhs.Y * rhs.Z) - (lhs.Z * rhs.Y),
		(lhs.Z * rhs.X) - (lhs.X * rhs.Z),
		(lhs.X * rhs.Y) - (lhs.Y * rhs.X)
	};
	
	return result;
}

inline float Length(const Vec3& v)
{
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

inline float LengthSquared(const Vec3& v)
{
	return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
}

inline Vec3 Normalized(const Vec3& v)
{
	const float invLength = 1.0 / Length(v);
	
	Vec3 result =
	{ 
		v.X * invLength,
		v.Y * invLength,
		v.Z * invLength,
	};
	
	return result;
}

inline float Distance(const Vec3& v1, const Vec3& v2)
{
	return Length(v1 - v2);
}

inline float DistanceSquared(const Vec3& v1, const Vec3& v2)
{
	return Length(v1 - v2);
}

inline Vec3 Lerp(const Vec3& v1, const Vec3& v2, const float amount)
{
	const float diff = 1.0f - amount;
	
	Vec3 result =
	{ 
		diff * v1.X + amount * v2.X,
		diff * v1.Y + amount * v2.Y,
		diff * v1.Z + amount * v2.Z
	};
		
	return result;
}

//TODO: Rotated


// Matrix2x2

inline Matrix2x2 operator*(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
	Matrix2x2 result = { 0 };
	result.a11 = lhs.a11 * rhs.a11 + lhs.a12 * rhs.a21;
	result.a12 = lhs.a11 * rhs.a12 + lhs.a12 * rhs.a22;
	result.a21 = lhs.a21 * rhs.a11 + lhs.a22 * rhs.a21;
	result.a22 = lhs.a21 * rhs.a12 + lhs.a22 * rhs.a22;
	return result;
}

// Matrix3x3

inline Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	Matrix3x3 result = { 0 };
	result.a11 = lhs.a11 * rhs.a11 + lhs.a12 * rhs.a21 + lhs.a13 * rhs.a31;
	result.a12 = lhs.a11 * rhs.a12 + lhs.a12 * rhs.a22 + lhs.a13 * rhs.a32;
	result.a13 = lhs.a11 * rhs.a13 + lhs.a12 * rhs.a23 + lhs.a13 * rhs.a33;
	result.a21 = lhs.a21 * rhs.a11 + lhs.a22 * rhs.a21 + lhs.a23 * rhs.a31;
	result.a22 = lhs.a21 * rhs.a12 + lhs.a22 * rhs.a22 + lhs.a23 * rhs.a32;
	result.a23 = lhs.a21 * rhs.a13 + lhs.a22 * rhs.a23 + lhs.a23 * rhs.a33;
	result.a31 = lhs.a31 * rhs.a11 + lhs.a32 * rhs.a21 + lhs.a33 * rhs.a31;
	result.a32 = lhs.a31 * rhs.a12 + lhs.a32 * rhs.a22 + lhs.a33 * rhs.a32;
	result.a33 = lhs.a31 * rhs.a13 + lhs.a32 * rhs.a23 + lhs.a33 * rhs.a33;
	return result;
}

// Matrix4x4

inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 result = { 0 };

	// First row
	result.a11 = lhs.a11 * rhs.a11 + lhs.a12 * rhs.a21 + lhs.a13 * rhs.a31 + lhs.a14 * rhs.a41;
	result.a12 = lhs.a11 * rhs.a12 + lhs.a12 * rhs.a22 + lhs.a13 * rhs.a32 + lhs.a14 * rhs.a42;
	result.a13 = lhs.a11 * rhs.a13 + lhs.a12 * rhs.a23 + lhs.a13 * rhs.a33 + lhs.a14 * rhs.a43;
	result.a14 = lhs.a11 * rhs.a14 + lhs.a12 * rhs.a24 + lhs.a13 * rhs.a34 + lhs.a14 * rhs.a44;

	// Second row
	result.a21 = lhs.a21 * rhs.a11 + lhs.a22 * rhs.a21 + lhs.a23 * rhs.a31 + lhs.a24 * rhs.a41;
	result.a22 = lhs.a21 * rhs.a12 + lhs.a22 * rhs.a22 + lhs.a23 * rhs.a32 + lhs.a24 * rhs.a42;
	result.a23 = lhs.a21 * rhs.a13 + lhs.a22 * rhs.a23 + lhs.a23 * rhs.a33 + lhs.a24 * rhs.a43;
	result.a24 = lhs.a21 * rhs.a14 + lhs.a22 * rhs.a24 + lhs.a23 * rhs.a34 + lhs.a24 * rhs.a44;

	// Third row
	result.a31 = lhs.a31 * rhs.a11 + lhs.a32 * rhs.a21 + lhs.a33 * rhs.a31 + lhs.a34 * rhs.a41;
	result.a32 = lhs.a31 * rhs.a12 + lhs.a32 * rhs.a22 + lhs.a33 * rhs.a32 + lhs.a34 * rhs.a42;
	result.a33 = lhs.a31 * rhs.a13 + lhs.a32 * rhs.a23 + lhs.a33 * rhs.a33 + lhs.a34 * rhs.a43;
	result.a34 = lhs.a31 * rhs.a14 + lhs.a32 * rhs.a24 + lhs.a33 * rhs.a34 + lhs.a34 * rhs.a44;

	// Fourth row
	result.a41 = lhs.a41 * rhs.a11 + lhs.a42 * rhs.a21 + lhs.a43 * rhs.a31 + lhs.a44 * rhs.a41;
	result.a42 = lhs.a41 * rhs.a12 + lhs.a42 * rhs.a22 + lhs.a43 * rhs.a32 + lhs.a44 * rhs.a42;
	result.a43 = lhs.a41 * rhs.a13 + lhs.a42 * rhs.a23 + lhs.a43 * rhs.a33 + lhs.a44 * rhs.a43;
	result.a44 = lhs.a41 * rhs.a14 + lhs.a42 * rhs.a24 + lhs.a43 * rhs.a34 + lhs.a44 * rhs.a44;

	return result;
}

// Quaternion

inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
	const Quaternion result =
	{ 
		lhs.X + rhs.X, 
		lhs.Y + rhs.Y, 
		lhs.Z + rhs.Z, 
		lhs.W + rhs.W
	};
	
	return result;
}

inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
	const Quaternion result =
	{ 
		lhs.X - rhs.X, 
		lhs.Y - rhs.Y, 
		lhs.Z - rhs.Z, 
		lhs.W - rhs.W
	};
	
	return result;
}

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
	const Quaternion result =
	{ 
		lhs.W * rhs.X + lhs.X * rhs.W + lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.W * rhs.Y + lhs.Y * rhs.W + lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.W * rhs.Z + lhs.Z * rhs.W + lhs.X * rhs.Y - lhs.Y * rhs.X,
		lhs.W * rhs.W - lhs.X * rhs.X - lhs.Y * rhs.Y - lhs.Z * rhs.Z
	};
	
	return result;
}

inline Quaternion operator*(const Quaternion& lhs, const float rhs)
{
	const Quaternion result =
	{ 
		lhs.X * rhs,
		lhs.Y * rhs, 
		lhs.Z * rhs, 
		lhs.W * rhs
	};
	
	return result;
}

inline Quaternion operator*(const float lhs, const Quaternion& rhs)
{
	const Quaternion result =
	{ 
		rhs.X * lhs,
		rhs.Y * lhs, 
		rhs.Z * lhs, 
		rhs.W * lhs
	};
	
	return result;
}

inline float Length(const Quaternion& q)
{
	return sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
}

inline float LengthSquared(const Quaternion& q)
{
	return q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W;
}

inline Quaternion Normalized(const Quaternion& q)
{
	const float invLength = 1.0 / Length(q);
	const Quaternion result = 
	{
		q.X * invLength, 
		q.Y * invLength, 
		q.Z * invLength, 
		q.W * invLength
	};
	
	return result;
}

inline Quaternion Rotated(const Quaternion& q, 
	const float ax, const float ay, const float az, const float angle)
{
	const float halfRad = angle * PI / 360;
	const float halfSin = sin(halfRad);
	const float halfCos = cos(halfRad);

	const float invLength = 1.0 / sqrt(ax * ax + ay * ay + az * az);

	const float rx = -ax * invLength * halfSin;
	const float ry = -ay * invLength * halfSin;
	const float rz = -az * invLength * halfSin;
	const float rw = halfCos;
	
	const Quaternion result = 
	{
		rw * q.X + rx * q.W + ry * q.Z - rz * q.Y,
		rw * q.Y + ry * q.W + rz * q.X - rx * q.Z,
		rw * q.Z + rz * q.W + rx * q.Y - ry * q.X,
		rw * q.W - rx * q.X - ry * q.Y - rz * q.Z
	};
	
	return Normalized(result);
}

inline Quaternion Rotated(const Quaternion& q, 
	const Vec3& axis, const float angle)
{
	return Rotated(q, axis.X, axis.Y, axis.Z, angle);
}

inline float Dot(const Quaternion& lhs, const Quaternion& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

inline Quaternion Conjugate(const Quaternion& q)
{
	const Quaternion result = { -q.X, -q.Y, -q.Z, q.W };
	
	return result;
}

inline Quaternion Inverse(const Quaternion& q)
{
	const float invLengthSquared = 1.0 / LengthSquared(q);
	const Quaternion result =
	{ 
		-q.X * invLengthSquared, 
		-q.Y * invLengthSquared, 
		-q.Z * invLengthSquared, 
		q.W * invLengthSquared
	};
	
	return result;
}

inline Quaternion FromAxis(const float ax, const float ay, const float az, 
	const float angle)
{
	const float halfRad = angle * PI / 360;
	const float halfSin = sin(halfRad);
	const float halfCos = cos(halfRad);
	const Quaternion result = 
	{
		-ax * halfSin, 
		-ay * halfSin, 
		-az * halfSin, 
		halfCos
	};
	
	return Normalized(result);
}

inline Quaternion FromAxis(const Vec3& axis, const float angle)
{
	return FromAxis(axis.X, axis.Y, axis.Z, angle);
}


inline Quaternion FromEuler(const float x, const float y, const float z)
{
	const float rx = x * PI / 360;
	const float ry = y * PI / 360;
	const float rz = z * PI / 360;
	
	const float sinX = -sin(rx);
	const float sinY = -sin(ry);
	const float sinZ = -sin(rz);
	
	const float cosX = cos(rx);
	const float cosY = cos(ry);
	const float cosZ = cos(rz);
	
	const float sinXsinY = sinX * sinY;
	const float sinXcosY = sinX * cosY;
	const float cosXcosY = cosX * cosY;
	const float cosXsinY = cosX * sinY;
	
	const Quaternion result =
	{ 
		cosXsinY * sinZ + sinXcosY * cosZ, 
		cosXsinY * cosZ + sinXcosY * sinZ, 
		cosXcosY * sinZ - sinXsinY * cosZ, 
		cosXcosY * cosZ - sinXsinY * sinZ
	};
	
	return result;
}

inline Quaternion FromEuler(const Vec3& v)
{
	return FromEuler(v.X, v.Y, v.Z);
}

inline Matrix4x4 ToMatrix4x4(const Quaternion& q)
{
	const float xx2 = 2 * q.X * q.X;
	const float xy2 = 2 * q.X * q.Y;
	const float xz2 = 2 * q.X * q.Z;
	const float xw2 = 2 * q.X * q.W;
	const float yy2 = 2 * q.Y * q.Y;
	const float yz2 = 2 * q.Y * q.Z;
	const float yw2 = 2 * q.Y * q.W;
	const float zz2 = 2 * q.Z * q.Z;
	const float zw2 = 2 * q.Z * q.W;
	
	const Matrix4x4 result =
	{ 
		1.0f - (yy2 + zz2), xy2 + zw2, xz2 - yw2, 0.0f,
		xy2 - zw2, 1.0f - (xx2 + zz2), yz2 + xw2, 0.0f,
		xz2 + yw2, yz2 - xw2, 1.0f - (xx2 + yy2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	
	return result;
}

inline Vec3 Forward(const Quaternion& q)
{
	const Vec3 result =
	{
		2.0f * q.X * q.Z + 2.0f * q.Y * q.W,
		2.0f * q.Y * q.X - 2.0f * q.X * q.W,
		1.0f - (2.0f * q.X * q.X + 2.0f * q.Y * q.Y)
	};
	
	return result;
}

inline Vec3 Backward(const Quaternion& q)
{
	const Vec3 result =
	{
		-2.0f * q.X * q.Z - 2.0f * q.Y * q.W,
		-2.0f * q.Y * q.X + 2.0f * q.X * q.W,
		-1.0f + (2.0f * q.X * q.X + 2.0f * q.Y * q.Y)
	};
	
	return result;
}

inline Vec3 Up(const Quaternion& q)
{
	const Vec3 result =
	{
		2.0f * q.X * q.Y - 2.0f * q.Z * q.W,
		1.0f - (2.0f * q.X * q.X + 2.0f * q.Z * q.Z),
		2.0f * q.Y * q.Z + 2.0f * q.X * q.W
	};
	
	return result;
}

inline Vec3 Down(const Quaternion& q)
{
	const Vec3 result =
	{
		-2.0f * q.X * q.Y + 2.0f * q.Z * q.W,
		-1.0f + (2.0f * q.X * q.X + 2.0f * q.Z * q.Z),
		-2.0f * q.Y * q.Z - 2.0f * q.X * q.W
	};
	
	return result;
}

inline Vec3 Left(const Quaternion& q)
{
	const Vec3 result =
	{
		1.0f - (2.0f * q.Y * q.Y + 2.0f * q.Z * q.Z),
		2.0f * q.X * q.Y + 2.0f * q.Z * q.W,
		2.0f * q.X * q.Z - 2.0f * q.Y * q.W
	};
	
	return result;
}

inline Vec3 Right(const Quaternion& q)
{
	const Vec3 result =
	{
		-1.0f + (2.0f * q.Y * q.Y + 2.0f * q.Z * q.Z),
		-2.0f * q.X * q.Y - 2.0f * q.Z * q.W,
		-2.0f * q.X * q.Z + 2.0f * q.Y * q.W
	};
	
	return result;
}