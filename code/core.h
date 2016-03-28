#pragma once

// Author: Tom

namespace core
{
	using namespace math;
	
	struct Transform
	{
		Vec3 position;
		Vec3 scale;
		Quaternion orientation;
	
		inline void MoveTowards(const Vec3& direction, const float amount)
		{
			position = position + (Normalized(direction) * amount);
		}
		
		inline void Rotate(const Quaternion& amount)
		{
			orientation = amount * orientation;
		}
		
		inline void Rotate(const Vec3& axis, const float angle)
		{
			orientation = Rotated(orientation, axis, angle);
		}
	
		inline void Rotate(const Vec3& euler)
		{
			orientation = FromEuler(euler) * orientation;
		}
		
		inline void Rotate(const float x, const float y, const float z)
		{
			orientation = FromEuler(x, y, z) * orientation;
		}
		
		inline Matrix4x4 Model() const
		{
			const Matrix4x4 translation = Translate(position);
			const Matrix4x4 rotation = CreateMatrix4x4(orientation);
			const Matrix4x4 scale = Scale(this->scale);	
		
			return translation * rotation * scale;	
		}
	};
	
	// TODO: Camera API
	struct Camera
	{
		Matrix4x4 viewProjection;	
	};
	
	inline Matrix4x4 MVP(const Transform& transform, const Camera& camera)
	{
		return camera.viewProjection * transform.Model();
	}
	
	inline Camera CreateOrthoCamera(const Transform& transform,
		const float width, const float height, const float zNear, const float zFar)
	{
		const Camera result =
		{
			Ortho(0, width, 0, height, zNear, zFar) * 
			ViewMatrix4x4(transform.position, transform.orientation)
		};
		
		return result;
	}
	
	inline Camera CreatePerspectiveCamera(const Transform& transform,
		const float fovy, const float width, const float height,
		const float zNear, const float zFar)
	{
		const Camera result =
		{
			Perspective(fovy, width/height, zNear, zFar) * 
			ViewMatrix4x4(transform.position, transform.orientation)
		};
		
		return result;
	}
	
	inline void UpdateCamera(Camera* camera, const Transform& transform)
	{
		
	}
}