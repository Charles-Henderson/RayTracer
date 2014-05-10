#ifndef SPHERE_H
#define SPHERE_H
#include <glm\vec3.hpp>
#include "Object.h"
#include <glm\gtx\normalize_dot.hpp>

class Sphere : public Object
{
public:
	Sphere(double x, double y, double z, double r, glm::vec3 color, double transparency, double reflectivity);
	ColorObject IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 light_p);
	double Transparency();
	double Reflectivity();
private:
	glm::vec3 GetDiffuseColor(glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const);
	glm::vec3 GetAmbientColor(float ambient_coef);
	glm::vec3 GetNormalFromPoint(glm::vec3 point_p);

	double mX, mY, mZ, mR, mTransparency, mReflectivity;
	glm::vec3 mColor;
};

#endif
