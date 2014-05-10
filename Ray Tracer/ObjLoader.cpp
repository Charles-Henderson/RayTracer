#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <glm\gtx\normalize_dot.hpp>
#include <algorithm>

ObjLoader::ObjLoader(std::string path, float z_plane_compensation)
{
	//# is comment
	//v is vertex
	//vn is vertex normal
	//f is face v//vn is syntax for vertex/vertex normal index. these start at 1 instead of 0
	mColor = glm::vec3(1.0, 1.0, 1.0);
	std::string line;
	std::string value;
	std::ifstream my_file(path);
	if (my_file.is_open())
	{
		while (std::getline(my_file, line))
		{
			std::size_t location = line.find_first_of(" ");
			value = line.substr(0, location);
			std::size_t old_location = location+1;
			if (value == "v" || value == "vn")
			{
				glm::vec3 vertex;
				for (int i = 0; i < 3; i++)
				{
					location = line.find_first_of(" ", old_location);
					std::string temp_value = line.substr(old_location, location - old_location);
					vertex[i] = atof(temp_value.c_str());
					old_location = location + 1;
					if (i == 2)
						vertex[i] += z_plane_compensation;
				}
				if (value == "v")
					mVertices.push_back(vertex);
				else
					mNormals.push_back(vertex);
			}
			else if (value == "f")
			{
				face vertex;
				for (int i = 0; i < 3; i++)
				{
					location = line.find_first_of(" ", old_location);
					std::string temp_value = line.substr(old_location, location - old_location);
					std::size_t slash_loc = temp_value.find_first_of("/");
					int v_index = atoi(temp_value.substr(0, slash_loc).c_str());
					int n_index = atoi(temp_value.substr(slash_loc + 2).c_str());
					vertex.vertex_indexes[i] = v_index-1; //minus 1 because these come in starting with position 1 where the array indexing starts at 0
					vertex.normal_indexes[i] = n_index-1;
					old_location = location + 1;
				}
				mFaces.push_back(vertex);
			}
		}
		my_file.close();
	}
	else
	{
		std::cout << "Error Opening file: " + path << std::endl;
	}

}


ColorObject ObjLoader::IntersectionPoint(glm::vec3 eye_p, glm::vec3 ray_v, glm::vec3 light_p)
{
//	glm::vec3 point_p, current_point;
	Triangle closest_face, current_face;
	bool first = true;
	for (int i = 0; i < mFaces.size(); i++)
	{
		glm::vec3 p1, p2, p3;
		p1 = mVertices[mFaces[i].vertex_indexes[0]];
		p2 = mVertices[mFaces[i].vertex_indexes[1]];
		p3 = mVertices[mFaces[i].vertex_indexes[2]];
		Triangle my_face(p1, p2, p3);
		bool intersect = my_face.IntersectionPoint(eye_p, ray_v);
		if (intersect)
		{
			current_face = my_face;
			if (first)
			{
				closest_face = current_face;
				first = false;
			}
			else if (current_face.GetIntersectionPoint().z < closest_face.GetIntersectionPoint().z)
			{
				closest_face = current_face;
			}
		}
	}

	if (first)
		return INVALID_COLOR_OBJECT;
	else
	{
		glm::vec3 ambient_color = GetAmbientColor(0.2);
		glm::vec3 diffuse_color = GetDiffuseColor(closest_face.GetNormal(), light_p, closest_face.GetIntersectionPoint(), 0.2, 1.0 - 0.2);
		return ColorObject(ambient_color, diffuse_color, closest_face.GetIntersectionPoint(), true);
	}
}

glm::vec3 ObjLoader::GetDiffuseColor(glm::vec3 normal, glm::vec3 light_p, glm::vec3 point_p, float ambient_const, float diffuse_const)
{
	double inverse_angle = glm::normalizeDot(normal, light_p - point_p);
	inverse_angle = std::max(inverse_angle, 0.0);
	inverse_angle = inverse_angle*inverse_angle;

	return float(ambient_const + inverse_angle*diffuse_const)*mColor;
}

glm::vec3 ObjLoader::GetAmbientColor(float ambient_coef)
{
	return mColor*ambient_coef;
}

double ObjLoader::Transparency(){ return 0; }
double ObjLoader::Reflectivity(){ return 0; }