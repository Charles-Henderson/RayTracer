#include "Sphere.h"
#include <algorithm>
#include <iostream>
#define M_PI 3.14159265358979323846

Sphere::Sphere(double x, double y, double z, double r, glm::vec3 color, double transparency, double reflectivity) :
mX(x),
mY(y),
mZ(z),
mR(r),
mColor(color),
mTransparency(transparency),
mReflectivity(reflectivity)
{}

ColorObject Sphere::IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 light_p)
{
	glm::vec3 eye_point_vector = glm::vec3(mX, mY, mZ) - eye_p;
	double vector_angle = glm::dot(eye_point_vector, ray_v);
	double self_dot = glm::dot(eye_point_vector, eye_point_vector);
	glm::vec3 p_p;
	if (vector_angle < 0.0)
		return INVALID_COLOR_OBJECT;

	double distance = (mR*mR) - ((self_dot - (vector_angle*vector_angle)));
	if (distance < 0.0)
		return INVALID_COLOR_OBJECT;
	else
	{
		distance = sqrt(distance);
		p_p = eye_p + (float(vector_angle - distance) * ray_v);
	}
	glm::vec3 ambient_color = GetAmbientColor(0.2);
	glm::vec3 diffuse_color = GetDiffuseColor(light_p, p_p, 0.2,1.0 - 0.2);
	return ColorObject(ambient_color, diffuse_color, p_p, true); 
}


glm::vec3 Sphere::GetNormalFromPoint(glm::vec3 point_p)
{
	return glm::normalize(point_p - glm::vec3(mX, mY, mZ));
}

glm::vec3 Sphere::GetDiffuseColor(glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const)
{

	double inverse_angle = glm::normalizeDot(GetNormalFromPoint(point_p), light_p - point_p);
	inverse_angle = std::max(inverse_angle, 0.0);
	inverse_angle = inverse_angle*inverse_angle;
	
	return float(ambient_const + inverse_angle*diffuse_const)*mColor;
}

glm::vec3 Sphere::GetAmbientColor(float ambient_coef)
{
	return mColor*ambient_coef;
}

double Sphere::Transparency()
{
	return mTransparency;
}

double Sphere::Reflectivity()
{
	return mReflectivity;
}
