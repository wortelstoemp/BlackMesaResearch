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
		
		inline Matrix4x4 CalculateModel() const
		{
			const Matrix4x4 translation = Translate(position);
			const Matrix4x4 rotation = CreateMatrix4x4(orientation);
			const Matrix4x4 scale = Scale(this->scale);	
		
			return translation * rotation * scale;	
		}
	};
	
	union Camera
	{
		enum CameraType
		{
			CAMERA_ORTHO,
			CAMERA_PERSPECTIVE,	
		};
		
		// Perspective
		struct
		{
			Transform transform;
			Matrix4x4 viewProjection;
			CameraType cameraType;
			float fovy;
			float aspect;
			float zNear;
			float zFar;
		};
		
		// Ortho
		struct
		{
			Transform transform;
			Matrix4x4 viewProjection;
			CameraType cameraType;
			float width;
			float height;
			float zNear;
			float zFar;
		};
		
		inline static Camera CreateOrtho(const Transform& transform,
			const float width, const float height, 
			const float zNear, const float zFar)
		{
			Camera result;
			result.transform = transform;
			result.viewProjection =
				Ortho(0, width, 0, height, zNear, zFar) * 
				ViewMatrix4x4(transform.position, transform.orientation);
			result.cameraType = CAMERA_ORTHO;
		
			return result;
		}
		
		inline static Camera CreatePerspective(const Transform& transform,
			const float fovy, const float aspect, const float zNear, const float zFar)
		{
			Camera result;
			result.transform = transform;
			result.viewProjection =
				Perspective(fovy, aspect, zNear, zFar) * 
				ViewMatrix4x4(transform.position, transform.orientation);
			result.cameraType = CAMERA_PERSPECTIVE;			
		
			return result;
		}
		
		inline void Update()
		{
			switch (cameraType)
			{
			case CAMERA_ORTHO:
				viewProjection = Ortho(0, width, 0, height, zNear, zFar) * 
					ViewMatrix4x4(transform.position, transform.orientation);
				break;
			case CAMERA_PERSPECTIVE:
				viewProjection = Perspective(fovy, aspect, zNear, zFar) * 
					ViewMatrix4x4(transform.position, transform.orientation);
				break;
			default:
				break;
			}
		}
	};
	
	inline Matrix4x4 CalculateMVP(const Transform& transform, const Camera& camera)
	{
		return camera.viewProjection * transform.CalculateModel();
	}
}