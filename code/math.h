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
 * Functions & operations *
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

inline Vec3 operator*(const Vec3& lhs, const float& rhs)
{
	Vec3 result = { lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs };
	return result;
}

inline Vec3 operator*(const float& lhs, const Vec3& rhs)
{
	return rhs * lhs;
}

inline Vec3 operator/(const Vec3& lhs, const float& rhs)
{
	Vec3 result = { lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs};
	return result;
}


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

//TODO(Tom): Quaternion

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
	const invLength = 1.0 / Length(q);
	const Quaternion result = 
	{
		q.X * invLength, 
		q.Y * invLength, 
		q.Z * invLength, 
		q.W * invLength
	};
	
	return result;
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

//TODO
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

inline Quaternion FromAxis(const Vec3& axis, const float angle)
{
	const float halfRad = angle * PI / 360;
	const float halfSin = sin(halfRad);
	const float halfCos = cos(halfRad);
	const Quaternion result = 
	{
		-axis.X * halfSin, 
		-axis.Y * halfSin, 
		-axis.Z * halfSin, 
		halfCos
	};
	
	return Normalized(result);
}

// FromEuler