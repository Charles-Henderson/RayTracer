#include "Triangle.h"
#include <glm\gtx\normalize_dot.hpp>


Triangle::Triangle(glm::vec3 one, glm::vec3 two, glm::vec3 three) :
point_1( one ),
point_2( two ),
point_3( three )
{

}

bool Triangle::IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 &point_p, float& u_coord, float& v_coord)
{ 
	glm::vec3 inner_triag_vector_1 = point_2 - point_1;
	glm::vec3 inner_triag_vector_2 = point_3 - point_1;
	glm::vec3 cross_v = glm::cross(ray_v, inner_triag_vector_2);
	double inner_v = glm::dot(inner_triag_vector_1, cross_v);

	if (inner_v > -0.00001 && inner_v < 0.00001)
		return(false);

	double falue = 1.0 / inner_v;
	glm::vec3 new_vec = eye_p - point_1;
	
	double final_u = falue * glm::dot(new_vec, cross_v);
	if (final_u < 0.0 || final_u > 1.0)
		return(false);

	glm::vec3 qalue = glm::cross(new_vec, inner_triag_vector_1);

	float final_v = falue * glm::dot(ray_v, qalue);
	if (final_v < 0.0 || final_u + final_v > 1.0)
		return(false);

	float final_t = falue * glm::dot(inner_triag_vector_2, qalue);
	if (final_t > 0.00001)
	{
		u_coord = final_u;
		v_coord = final_v;
		point_p = (float(1 - final_u - final_v)*point_1) + (float(final_u)*point_2) + (float(final_v)*point_3);
		return true;
	}
	return false; 
}