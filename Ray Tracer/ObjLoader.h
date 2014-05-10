#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include <vector>
#include <string>
#include <glm\vec3.hpp>
#include "Object.h"
#include "Triangle.h"

struct face
{
	glm::vec3 vertex_indexes;
	glm::vec3 normal_indexes;
};

class ObjLoader : public Object
{
public:
	ObjLoader(std::string path, float z_plane_compensation = 0);

	ColorObject IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 light_p);
	glm::vec3 GetNormalFromPoint(glm::vec3 point_p);
	glm::vec3 GetDiffuseColor(glm::vec3 normal, glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const);
	glm::vec3 GetAmbientColor(float ambient_coef);
	double Transparency();
	double Reflectivity();

private:
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<face> mFaces;
	glm::vec3 mColor;
};

#endif