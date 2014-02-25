// Ray Tracer.cpp : Defines the entry point for the console application.
//
#include <GL\glew.h>
#include <glut.h>
#include "DisplayEngine.h"

int main(int argc, char* argv[])
{

	int width = 512;
	int height = 512;
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitWindowSize(width, height);

	glutCreateWindow("Ray Tracer - Glut");

	GLenum glew_error = glewInit();
	if (GLEW_OK != glew_error)
	{
		return 0;
	}

	display::InitGl();
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutDisplayFunc(display::Draw);

	glutMainLoop();

	return 0;
}
