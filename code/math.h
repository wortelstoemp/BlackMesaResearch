#pragma once

// Author(s): Simon, Tom

const float PI = 3.14159265358979323846;

/*************
* Datatypes *
*************/

struct Vec2
{
	float X;
	float Y;
	
	inline static Vec2 PositiveXAxis()
	{
		const Vec2 result = { 1.0f, 0.0f };
		return result;
	}

	inline static Vec2 NegativeXAxis()
	{
		const Vec2 result = { -1.0f, 0.0f };
		return result;
	}
	
	inline static Vec2 PositiveYAxis()
	{
		const Vec2 result = { 0.0f, 1.0f };
		return result;
	}

	inline static Vec2 NegativeYAxis()
	{
		const Vec2 result = { 0.0f, -1.0f };
		return result;
	}
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
	
	inline static Vec3 PositiveXAxis()
	{
		const Vec3 result = { 1.0f, 0.0f, 0.0f };
		return result;
	}

	inline static Vec3 NegativeXAxis()
	{
		const Vec3 result = { -1.0f, 0.0f, 0.0f };
		return result;
	}
	
	inline static Vec3 PositiveYAxis()
	{
		const Vec3 result = { 0.0f, 1.0f, 0.0f };
		return result;
	}

	inline static Vec3 NegativeYAxis()
	{
		const Vec3 result = { 0.0f, -1.0f, 0.0f };
		return result;
	}
	
	inline static Vec3 PositiveZAxis()
	{
		const Vec3 result = { 0.0f, 0.0f, 1.0f };
		return result;
	}

	inline static Vec3 NegativeZAxis()
	{
		const Vec3 result = { 0.0f, 0.0f, -1.0f };
		return result;
	}
};

union Vec4
{
	struct
	{
		Vec2 XY;
		float Z;
		float W;
	};
	struct
	{
		Vec3 XYZ;
		float W;
	};	
	struct
	{
		float X;
		float Y;
		float Z;
		float W;		
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
	const Vec2 result = { lhs.X + rhs.X, lhs.Y + rhs.Y };
	return result;
}

inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
	const Vec2 result = { lhs.X - rhs.X, lhs.Y - rhs.Y };
	return result;
}

inline Vec2 operator-(const Vec2& v)
{
	const Vec2 result = { -v.X, -v.Y };
	return result;
}

inline Vec2 operator*(const Matrix2x2& lhs, const Vec2& rhs)
{
	const Vec2 result =
	{
		lhs.a11 * rhs.X + lhs.a12 * rhs.Y,
		lhs.a21 * rhs.X + lhs.a22 * rhs.Y
	};
	
	return result;
}

// Vec3

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
	const Vec3 result = { lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z};
	return result;
}

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
	const Vec3 result = { lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z};
	return result;
}

inline Vec3 operator-(const Vec3& v)
{
	const Vec3 result = { -v.X, -v.Y, -v.Z };
	return result;
}

inline Vec3 operator*(const Matrix3x3& lhs, const Vec3& rhs)
{
	const Vec3 result =
	{ 
		lhs.a11 * rhs.X + lhs.a12 * rhs.Y + lhs.a13 * rhs.Z,
		lhs.a21 * rhs.X + lhs.a22 * rhs.Y + lhs.a23 * rhs.Z,
		lhs.a31 * rhs.X + lhs.a32 * rhs.Y + lhs.a33 * rhs.Z
	};
	
	return result;
}

inline Vec3 operator*(const Matrix4x4 lhs, const Vec3& rhs)
{
	const Vec3 result =
	{ 
		lhs.a11 * rhs.X + lhs.a12 * rhs.Y + lhs.a13 * rhs.Z + lhs.a14,
		lhs.a21 * rhs.X + lhs.a22 * rhs.Y + lhs.a23 * rhs.Z + lhs.a24,
		lhs.a31 * rhs.X + lhs.a32 * rhs.Y + lhs.a33 * rhs.Z + lhs.a34
	};
	
	return result;
}

inline Vec3 operator*(const Vec3& lhs, const float rhs)
{
	const Vec3 result = { lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs };
	return result;
}

inline Vec3 operator*(const float lhs, const Vec3& rhs)
{
	return rhs * lhs;
}

inline Vec3 operator/(const Vec3& lhs, const float& rhs)
{
	const Vec3 result = { lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs};
	return result;
}

inline float Dot(const Vec3& lhs, const Vec3& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

inline Vec3 Cross(const Vec3& lhs, const Vec3& rhs)
{
	const Vec3 result =
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
	
	const Vec3 result =
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
	return LengthSquared(v1 - v2);
}

inline Vec3 Lerp(const Vec3& v1, const Vec3& v2, const float amount)
{
	const float diff = 1.0f - amount;
	
	const Vec3 result =
	{ 
		diff * v1.X + amount * v2.X,
		diff * v1.Y + amount * v2.Y,
		diff * v1.Z + amount * v2.Z
	};
		
	return result;
}

//TODO: Rotated

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

inline Matrix4x4 CreateMatrix4x4()
{
	const Matrix4x4 result =
	{ 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1  
	};
	
	return result;
}

inline Matrix4x4 CreateMatrix4x4(const float d)
{
	const Matrix4x4 result =
	{ 
		d, 0, 0, 0,
		0, d, 0, 0,
		0, 0, d, 0,
		0, 0, 0, d  
	};
	
	return result;
}

inline Matrix4x4 CreateMatrix4x4(const Quaternion& q)
{
	const float xx2 = 2.0f * q.X * q.X;
	const float xy2 = 2.0f * q.X * q.Y;
	const float xz2 = 2.0f * q.X * q.Z;
	const float xw2 = 2.0f * q.X * q.W;
	const float yy2 = 2.0f * q.Y * q.Y;
	const float yz2 = 2.0f * q.Y * q.Z;
	const float yw2 = 2.0f * q.Y * q.W;
	const float zz2 = 2.0f * q.Z * q.Z;
	const float zw2 = 2.0f * q.Z * q.W;
	
	const Matrix4x4 result =
	{ 
		1.0f - (yy2 + zz2), xy2 + zw2, xz2 - yw2, 0.0f,
		xy2 - zw2, 1.0f - (xx2 + zz2), yz2 + xw2, 0.0f,
		xz2 + yw2, yz2 - xw2, 1.0f - (xx2 + yy2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	
	return result;
}

inline Matrix4x4 Transpose(const Matrix4x4& m)
{
	const Matrix4x4 result =
	{ 
		m.a11, m.a21, m.a31, m.a41,
		m.a12, m.a22, m.a32, m.a42,
		m.a13, m.a23, m.a33, m.a43,
		m.a14, m.a24, m.a34, m.a44  
	};
	
	return result;
}

inline Matrix4x4 Translate(const Matrix4x4& m, const Vec3& v)
{
	const Matrix4x4 translate =
	{ 
		1, 0, 0, v.X,
		0, 1, 0, v.Y,
		0, 0, 1, v.Z,
		0, 0, 0, 1  
	};
	
	return translate * m;
}

inline Matrix4x4 Translate(const Vec3& v)
{
	const Matrix4x4 result =
	{ 
		1, 0, 0, v.X,
		0, 1, 0, v.Y,
		0, 0, 1, v.Z,
		0, 0, 0, 1  
	};
	
	return result;
}

inline Matrix4x4 Scale(const Matrix4x4& m, const Vec3& v)
{
	const Matrix4x4 scale =
	{ 
		v.X, 0, 0, 0,
		0, v.Y, 0, 0,
		0, 0, v.Z, 0,
		0, 0, 0, 1  
	};
	
	return scale * m;
}

inline Matrix4x4 Scale(const Vec3& v)
{
	const Matrix4x4 result =
	{ 
		v.X, 0, 0, 0,
		0, v.Y, 0, 0,
		0, 0, v.Z, 0,
		0, 0, 0, 1  
	};
	
	return result;
}

inline Matrix4x4 Mirror(const Matrix4x4& m)
{
	const Matrix4x4 scale =
	{ 
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1  
	};
	
	return scale * m;
}

inline Matrix4x4 Mirror()
{
	const Matrix4x4 result =
	{ 
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1  
	};
	
	return result;
}

inline Matrix4x4 Ortho(const float left, const float right,
	const float bottom, const float top, 
	const float zNear, const float zFar)
{
	const float width = right - left;
	const float height = top - bottom;
	const float depth = zFar - zNear;
	
	const Matrix4x4 result =
	{ 
		2.0f/width, 0, 0, -(right + left) / width,
		0, 2.0f/height, 0, -(top + bottom) / height,
		0, 0, -2.0f/depth, -(zFar + zNear) / depth,
		0, 0, 0, 0  
	};
	
	return result;		
}

inline Matrix4x4 Perspective(const float fovy, const float aspect,
	const float zNear, const float zFar)
{
	const float angle = (fovy / 2.0f) * PI / 180.0f;
	const float yScale = 1.0f / tan(angle);
	const float xScale = yScale / aspect;
	const float depth = zFar - zNear;
	
	const Matrix4x4 result =
	{ 
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, (zNear + zFar) / depth, -2.0f * zNear * zFar / depth,
		0, 0, 1, 0  
	};
	
	return result;
}

inline Matrix4x4 LookAt(const Vec3& eye, const Vec3& look, const Vec3& up)
{
	const Vec3 l = Normalized(look);
	const Vec3 r = Cross(look, up);
	const Vec3 u = Normalized(Cross(l, r));
	
	const Matrix4x4 result =
	{ 
		r.X, u.X, -l.X, -r.X * eye.X - u.X * eye.Y + l.X * eye.Z,
		r.Y, u.Y, -l.Y, -r.Y * eye.X - u.Y * eye.Y + l.Y * eye.Z,
		r.Z, u.Z, -l.Z, -r.Z * eye.X - u.Z * eye.Y + l.Z * eye.Z,
		0, 0, 0, 1, 
	};
	
	return result;
}

inline Matrix4x4 ViewMatrix4x4(const Vec3& position, const Quaternion& orientation)
{
	const Vec3 r = Right(orientation);
	const Vec3 u = Up(orientation);
	const Vec3 f = Forward(orientation);
	
	const Matrix4x4 result =
	{ 
		r.X, r.Y, r.Z, -r.X * position.X - r.Y * position.Y - r.Z * position.Z,
		u.X, u.Y, u.Z, -u.X * position.X - u.Y * position.Y - u.Z * position.Z,
		f.X, f.Y, f.Z, -f.X * position.X - f.Y * position.Y - f.Z * position.Z,
		0, 0, 0, 1  
	};
	
	return result;
}