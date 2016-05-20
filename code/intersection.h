#pragma once

struct IntersectionData
{
	float distance;	// to pick nearest OBB
	bool intersects;
};

struct Ray
{
	Vec3 origin;
	Vec3 direction;
};

struct AABB
{
	Vec3 min;
	Vec3 max;
};

struct OBB
{
	Transform transform;
	AABB aabb;	
};

// Based on Ray-AABB from Real-Time Collision Detection by Christer Ericson
IntersectionData IntersectRayOBB(const Ray& ray, const OBB& obb)
{
	float tMin = 0.0f;
	float tMax = FLT_MAX;
	Vec3 obbPositionWorldspace = { obb.transform.position.x, obb.transform.position.y, obb.position.z };
	Vec3 delta = obbPositionWorldspace - ray.origin;
	Vec3 axis[3] = {
		Left(obb.transform.orientation),	// x-axis
		Up(obb.transform.orientation),		// y-axis
		Forward(obb.transform.orientation)	// z-axis
	};
	IntersectionData result = { 0.0f, false };
	
	// For every axis of OBB: test if there is intersection 
	// with 2 planes perpendicular to this axis.
	for (int i = 0; i < 3; i++)
	{
		float e = Dot(axis[i], delta);
		float f = Dot(ray.direction, axis[i]);
		
		if (fabs(f) > 0.000001f)
		{
			float t1 = (e + obb.aabb.min.values[i]) / f;
			float t2 = (e + obb.aabb.max.values[i]) / f;
	
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}
	
			if (t1 > tMin) 
				tMin = t1;
			if (t2 < tMax)
				tMax = t2;
			if (tMax < tMin )
				return result;
		}
		else
		{
			if (obb.aabb.min.values[i] - e > 0.0f || obb.aabb.max.values[i] - e < 0.0f)
				return result;
		}
	}
	
	result.distance = tMin;
	result.intersects = true;
	
	return result;
}