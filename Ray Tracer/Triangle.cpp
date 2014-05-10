#include "Triangle.h"
#include <glm\gtx\normalize_dot.hpp>


Triangle::Triangle(glm::vec3 one, glm::vec3 two, glm::vec3 three) :
mPoint1( one ),
mPoint2( two ),
mPoint3( three )
{

}

Triangle::Triangle()
{

}

bool Triangle::IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v)
{ 
	glm::vec3 inner_triag_vector_1 = mPoint2 - mPoint1;
	glm::vec3 inner_triag_vector_2 = mPoint3 - mPoint1;
	glm::vec3 cross_v = glm::cross(ray_v, inner_triag_vector_2);
	double inner_v = glm::dot(inner_triag_vector_1, cross_v);

	if (inner_v > -0.00001 && inner_v < 0.00001)
		return(false);

	double falue = 1.0 / inner_v;
	glm::vec3 new_vec = eye_p - mPoint1;
	
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
		/*u_coord = final_u;
		v_coord = final_v;
		point_p = (float(1 - final_u - final_v)*mPoint1) + (float(final_u)*mPoint2) + (float(final_v)*mPoint3);*/
		mLastIntersectionPoint = (float(1 - final_u - final_v)*mPoint1) + (float(final_u)*mPoint2) + (float(final_v)*mPoint3);
		return true;
	}
	return false; 
}

glm::vec3 Triangle::GetIntersectionPoint()
{
	return mLastIntersectionPoint;
}


glm::vec3 Triangle::GetNormal()
{
	glm::vec3 vectorAB = glm::normalize(mPoint2 - mPoint1);
	glm::vec3 vectorAC = glm::normalize( mPoint3 - mPoint1);
	glm::vec3 normal = glm::cross(vectorAB, vectorAC);
	return glm::normalize(normal);
}