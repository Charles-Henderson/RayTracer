#include "DisplayEngine.h"
#include <vector>
#include <GL\glew.h>
#include <glut.h>
#include <math.h>
#include <glm\vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"
#include <iostream>
#define M_PI 3.14159265358979323846
glm::vec3 light_p(1.0, 1.0, -1.6);
std::vector<Object*> scene_objects;
float ambient_coef = 0.2;
float diffuse_coef = 1.0 - ambient_coef;
int width = 512;
int height = 512;
int count1 = 0;
int count2 = 0;
double step = 0;

void display::InitGl()
{
	scene_objects.push_back(new Sphere(0.0, 0.0, -1.8, 0.2, glm::vec3(0.7529, 0.027, 0.4117), 1.0, 1.0));
	scene_objects.push_back(new Sphere(0.4, 0.4, -1.6, 0.2, glm::vec3(1.0, 0.0, 1.0), 1.0, 1.0));
	scene_objects.push_back(new Sphere(-0.3, -0.4, -1.8, 0.2, glm::vec3(0.23, 0.027, 0.4117), 1.0, 1.0));
	scene_objects.push_back(new Sphere(0, -0.7, -1.6, 0.2, glm::vec3(0.0, 1.0, 1.0), 1.0, 1.0));

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
	int size = width * height * number_components;
	float *temp_data = new float[size];

	double fov_x = M_PI / 4;
	double fov_y = (height / float(width))*fov_x;
	glm::vec3 eye_p(0, 0, 0);

#pragma omp parallel for
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = (((2 * i) - float(width)) / float(width)) * tan(fov_x);
			float y = (((2 * j) - float(height)) / float(height)) * tan(fov_y);
			float z = -1;
			glm::vec3 ray_v = glm::normalize(glm::vec3(x, y, z) - eye_p);
			glm::vec3 color = TraceRay(eye_p, ray_v);

			temp_data[((j*width + i) * number_components) + 0] = color[0];
			temp_data[((j*width + i) * number_components) + 1] = color[1];
			temp_data[((j*width + i) * number_components) + 2] = color[2];
			temp_data[((j*width + i) * number_components) + 3] = 1.0;
		}
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, temp_data);
	glutPostRedisplay();
	light_p.z = -1 - 0.6*(cos(step) + 1) / 2;
	step += 1.0 / M_PI;
	//std::cout << "First intersection: " << count1 << "\nSecond intersection: " << count2 << std::endl;
}

void display::InterSectSpheres(glm::vec3 &point, Object **object, glm::vec3 eye_p, glm::vec3 ray_v, bool &intersect)
{
	glm::vec3 first_point, closest_point;
	bool first_run = true;
	intersect = false;
	for (int i = 0; i < scene_objects.size(); i++)
	{
		bool intersection = scene_objects[i]->IntersectionPoint(eye_p, ray_v, first_point);
		if (intersection && (first_point != eye_p))
		{
			intersect = true;
			if (first_point.z > closest_point.z || first_run)
			{
				point = first_point;
				*object = scene_objects[i];
				first_run = false;
			}
		}
	}
}

glm::vec3 display::TraceRay(glm::vec3 eye_p, glm::vec3 ray_v)
{
	glm::vec3 point_p;
	Object *first_object, *second_object;
	bool first_run = true;
	bool intersect = false;
	InterSectSpheres(point_p, &first_object, eye_p, ray_v, intersect);

	if (intersect)
	{
		eye_p = point_p;
		ray_v = light_p - point_p;
		ray_v = glm::normalize(ray_v);
		InterSectSpheres(point_p, &second_object, eye_p, ray_v, intersect);

		if (!intersect)
			return first_object->GetDiffuseColor(light_p, point_p, ambient_coef, diffuse_coef);
		else
			return first_object->GetAmbientColor(ambient_coef);
	}
	else
		return glm::vec3(0, 0, 0);
}