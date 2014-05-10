#ifndef OBJECT
#define OBJECT
#include <glm\vec3.hpp>

#define INVALID_COLOR_OBJECT ColorObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), false)

class ColorObject;

class Object 
{
public:
	virtual ColorObject IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 light_p) = 0;
};

class ColorObject
{
public:
	ColorObject(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 intersection_point, bool valid);
	glm::vec3 GetAmbientColor();
	glm::vec3 GetDiffuseColor();
	glm::vec3 GetIntersectionPoint();
	bool IsValid();
private:
	bool mIsValid;
	glm::vec3 mIntersectionPoint;
	glm::vec3 mAmbientColor;
	glm::vec3 mDiffuseColor;
};

/* easy copy-paste

bool IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 &point_p);
glm::vec3 GetNormalFromPoint(glm::vec3 point_p);
glm::vec3 GetColor();
glm::vec3 GetDiffuseColor(glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const);
glm::vec3 GetAmbientColor(float ambient_coef);
double Transparency();
double Reflectivity();


*/

#endif