#include "DisplayEngine.h"
#include <vector>
#include <GL\glew.h>
#include <glut.h>
#include <math.h>
#include <glm\vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"
#include "Triangle.h"
#include "ObjLoader.h"
#include "Object.h"
#include <iostream>
#define M_PI 3.14159265358979323846
glm::vec3 light_p(1.0, 1.0, -1.6);
std::vector<Object*> scene_objects;
float ambient_coef = 0.2;
float diffuse_coef = 1.0 - ambient_coef;
int count1 = 0;
int count2 = 0;
double step = 0;
int over_sample_factor = 1;
int window_width = 512;
int window_height = 512;
int texture_width = window_width*over_sample_factor;
int texture_height = window_height*over_sample_factor;



void display::InitGl()
{
	ObjLoader *cone = new ObjLoader("C:\\Users\\charl_000\\Documents\\Visual Studio 2013\\Projects\\Ray Tracer\\Meshes\\cone.obj", -2);
//	scene_objects.push_back(new Sphere(0.0, 0.0, -1.8, 0.2, glm::vec3(0.7529, 0.027, 0.4117), 1.0, 1.0));
///	scene_objects.push_back(new Sphere(0.4, 0.4, -1.6, 0.2, glm::vec3(1.0, 0.0, 1.0), 1.0, 1.0));
//	scene_objects.push_back(new Sphere(-0.3, -0.4, -1.8, 0.2, glm::vec3(0.23, 0.027, 0.4117), 1.0, 1.0));
//	scene_objects.push_back(new Sphere(0, -0.7, -1.6, 0.2, glm::vec3(0.0, 1.0, 1.0), 1.0, 1.0));
//	scene_objects.push_back((Object*) new Triangle(glm::vec3(-0.5, 0, -1.6), glm::vec3(0.5, 0, -1.6), glm::vec3(0, 0.5, -1.6)));
	scene_objects.push_back((Object*)cone);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	GLuint texture_id;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

}

void display::Draw()
{
	display::ComputeData();
	glClear(GL_COLOR);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);  glVertex2f(0, 0);
	glTexCoord2f(1, 0);  glVertex2f(1, 0);
	glTexCoord2f(1, 1);  glVertex2f(1, 1);
	glTexCoord2f(0, 1);  glVertex2f(0, 1);
	glEnd();

	glutSwapBuffers();
}

void display::ComputeData()
{
	int number_components = 4;
	int size = texture_width * texture_height * number_components;
	float *temp_data = new float[size];

	double fov_x = M_PI / 4;
	double fov_y = (texture_height / float(texture_width))*fov_x;
	glm::vec3 eye_p(0, 0, 0);

#pragma omp parallel for
	for (int i = 0; i < texture_width; i++)
	{
		for (int j = 0; j < texture_height; j++)
		{
			float x = (((2 * i) - float(texture_width)) / float(texture_width)) * tan(fov_x);
			float y = (((2 * j) - float(texture_height)) / float(texture_height)) * tan(fov_y);
			float z = -1;
			glm::vec3 ray_v = glm::normalize(glm::vec3(x, y, z) - eye_p);
			glm::vec3 color = TraceRay(eye_p, ray_v);

			temp_data[((j*texture_width + i) * number_components) + 0] = color[0];
			temp_data[((j*texture_width + i) * number_components) + 1] = color[1];
			temp_data[((j*texture_width + i) * number_components) + 2] = color[2];
			temp_data[((j*texture_width + i) * number_components) + 3] = 1.0;
		}
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_width, texture_height, GL_RGBA, GL_FLOAT, temp_data);
	glutPostRedisplay();
	//light_p.z = -1 - 0.6*(cos(step) + 1) / 2;
	//step += 1.0 / M_PI;

	delete[] temp_data;
	//std::cout << "First intersection: " << count1 << "\nSecond intersection: " << count2 << std::endl;
}

ColorObject display::InterSectSceneObjects(glm::vec3 eye_p, glm::vec3 ray_v)
{
	ColorObject closest_point(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), false);
	bool first_run = true;
	for (int i = 0; i < scene_objects.size(); i++)
	{
		ColorObject intersection = scene_objects[i]->IntersectionPoint(eye_p, ray_v, light_p);
		if (intersection.IsValid())
		{
			if ((intersection.GetIntersectionPoint().z > closest_point.GetIntersectionPoint().z) || first_run)
			{
				closest_point = intersection;
				first_run = false;
			}
		}
	}
	return closest_point;
}

glm::vec3 display::TraceRay(glm::vec3 eye_p, glm::vec3 ray_v)
{
	ColorObject first_intersection_point = InterSectSceneObjects(eye_p, ray_v);

	if (first_intersection_point.IsValid()) //for shadows
	{
		eye_p = first_intersection_point.GetIntersectionPoint();
		ray_v = light_p - first_intersection_point.GetIntersectionPoint();
		ray_v = glm::normalize(ray_v);
		ColorObject second_intersection_point = InterSectSceneObjects(eye_p, ray_v);

		if (!second_intersection_point.IsValid())
			return first_intersection_point.GetDiffuseColor();
		else
			return first_intersection_point.GetAmbientColor();
	}
	else
		return glm::vec3(0, 0, 0);
}