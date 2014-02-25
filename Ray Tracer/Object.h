#ifndef OBJECT
#define OBJECT
#include <glm\vec3.hpp>

class Object 
{
public:
	virtual bool IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 &point_p) = 0;
	virtual glm::vec3 GetNormalFromPoint(glm::vec3 point_p) = 0;
	virtual glm::vec3 GetColor() = 0;
	virtual glm::vec3 GetDiffuseColor(glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const) = 0;
	virtual glm::vec3 GetAmbientColor(float ambient_coef) = 0;
	virtual double Transparency() = 0;
	virtual double Reflectivity() = 0;
};


#endif