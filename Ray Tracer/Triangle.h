#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <glm\vec3.hpp>

class Triangle
{
public:
	Triangle(glm::vec3 one, glm::vec3 two, glm::vec3 three);
	Triangle();
	bool IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v);
	glm::vec3 GetNormal();
	glm::vec3 GetIntersectionPoint();
private:
	glm::vec3 mLastIntersectionPoint;
	glm::vec3 mPoint1, mPoint2, mPoint3;
};

#endif