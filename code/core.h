#pragma once

// Author(s): Tom

struct Transform
{
	Vec3 position;
	Vec3 scale;
	Quaternion orientation;

	inline void TranslateTowards(const Vec3& direction, const float amount)
	{
		position = position + (Normalized(direction) * amount);
	}
	
	inline void TranslateUp(const float amount)
	{
		TranslateTowards(Up(orientation), amount);
	}
	
	inline void TranslateDown(const float amount)
	{
		TranslateTowards(Down(orientation), amount);
	}
	
	inline void TranslateLeft(const float amount)
	{
		TranslateTowards(Left(orientation), amount);
	}
	
	inline void TranslateRight(const float amount)
	{
		TranslateTowards(Right(orientation), amount);
	}
	
	inline void TranslateForward(const float amount)
	{
		TranslateTowards(Forward(orientation), amount);
	}
	
	inline void TranslateBackward(const float amount)
	{
		TranslateTowards(Backward(orientation), amount);
	}
	
	inline void Rotate(const Quaternion& amount)
	{
		orientation = amount * orientation;
	}
	
	inline void Rotate(const Vec3& axis, const float angle)
	{
		orientation = Rotated(orientation, axis, angle);
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
		CAMERA_PERSPECTIVE,
		CAMERA_ORTHO,
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
	
	inline void CreatePerspective(const Transform& transform,
		const float fovy, const float aspect, const float zNear, const float zFar)
	{
		this->transform = transform;
		this->viewProjection =
			Perspective(fovy, aspect, zNear, zFar) * 
			ViewMatrix4x4(transform.position, transform.orientation);
		this->cameraType = CAMERA_PERSPECTIVE;
		this->fovy = fovy;
		this->aspect = aspect;
		this->zNear = zNear;
		this->zFar = zFar;
	}
	
	inline void CreateOrtho(const Transform& transform,
		const float width, const float height, 
		const float zNear, const float zFar)
	{
		this->transform = transform;
		this->viewProjection =
			Ortho(0, width, 0, height, zNear, zFar) * 
			ViewMatrix4x4(transform.position, transform.orientation);
		this->cameraType = CAMERA_ORTHO;
		this->width = width;
		this->height = height;
		this->zNear = zNear;
		this->zFar = zFar;
	}
	
	inline void Update()
	{
		switch (this->cameraType)
		{
		case CAMERA_PERSPECTIVE:
			this->viewProjection = Perspective(this->fovy, this->aspect, this->zNear, this->zFar) * 
				ViewMatrix4x4(this->transform.position, this->transform.orientation);
			break;
		case CAMERA_ORTHO:
			this->viewProjection = Ortho(0, this->width, 0, this->height, this->zNear, this->zFar) * 
				ViewMatrix4x4(this->transform.position, this->transform.orientation);
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