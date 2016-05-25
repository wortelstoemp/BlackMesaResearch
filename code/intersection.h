#pragma once

// Authors: Tom

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

// Based on Ray-AABB from Real-Time Collision Detection by Christer Ericson
IntersectionData IntersectRayOBB(const Ray& ray, const AABB& aabb, const Transform& transform)
{
	float tMin = 0.0f;
	float tMax = FLT_MAX;
	Vec3 delta = transform.position - ray.origin;
	Vec3 axis[3] = {
		Left(transform.orientation),	// x-axis
		Up(transform.orientation),		// y-axis
		Forward(transform.orientation)	// z-axis
	};
	AABB box;
	box.min = aabb.min; //* transform.scale;
	box.max = aabb.max; //* transform.scale;	
	IntersectionData result = { 0.0f, false };
	
	// For every axis of OBB: test if there is intersection 
	// with 2 planes perpendicular to this axis.
	for (int i = 0; i < 3; i++)
	{
		float e = Dot(axis[i], delta);
		float f = Dot(ray.direction, axis[i]);
		
		if (fabs(f) > 0.000001f)
		{
			float t1 = (e + box.min.values[i]) / f;
			float t2 = (e + box.max.values[i]) / f;
	
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
			if (box.min.values[i] - e > 0.0f || box.max.values[i] - e < 0.0f)
				return result;
		}
	}
	
	result.distance = tMin;
	result.intersects = true;
	
	return result;
}