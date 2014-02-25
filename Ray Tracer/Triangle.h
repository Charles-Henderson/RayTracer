#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <glm\vec3.hpp>

class Triangle
{
public:
	Triangle(glm::vec3 one, glm::vec3 two, glm::vec3 three);
	bool IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 &point_p, float& u_coord, float& v_coord);
private:
	glm::vec3 point_1, point_2, point_3;
};

#endif