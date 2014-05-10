#ifndef DISPLAY_ENGINE_H
#define DISPLAY_ENGINE_H
#include <glm\vec3.hpp>
#include "Object.h"

namespace display
{
	void Draw();
	void ComputeData();
	void InitGl();
	glm::vec3 TraceRay(glm::vec3 eye_p, glm::vec3 ray_v);
	ColorObject InterSectSceneObjects(glm::vec3 eye_p, glm::vec3 ray_v);
}

#endif