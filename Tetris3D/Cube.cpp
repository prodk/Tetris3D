// Cube.cpp - Cube class implementation.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Cube.h"

Cube::Cube(std::size_t idExt, vector_3d c, float sizeExt):
Shape(idExt, c)
{
	size = sizeExt;
}

Cube::~Cube(void)
{
}

void Cube::draw()
{
	glPushMatrix();
	material.setValues();
	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);	// Move the cube's center.
	glutWireCube(size);
	glPopMatrix();
}

float Cube::getSize() const
{
	return size;
}

void Cube::rotateX(float angle)
{
	// Modify the coordinates of teh cube's center.
	float yNew = (vCenter[1] - vOrigin[1])*std::cos(angle) - 
				(vCenter[2] - vOrigin[2])*std::sin(angle);
	float zNew = (vCenter[1] - vOrigin[1])*std::sin(angle) + 
				(vCenter[2] - vOrigin[2])*std::cos(angle);

	vCenter[1] = yNew + vOrigin[1];
	vCenter[2] = zNew + vOrigin[2];
}

void Cube::rotateY(float angle)
{
	// Modify the coordinates of teh cube's center.
	float xNew = (vCenter[0] - vOrigin[0])*std::cos(angle) - 
				(vCenter[2] - vOrigin[2])*std::sin(angle);
	float zNew = (vCenter[0] - vOrigin[0])*std::sin(angle) + 
				(vCenter[2] - vOrigin[2])*std::cos(angle);

	vCenter[0] = xNew + vOrigin[0];
	vCenter[2] = zNew + vOrigin[2];
}

void Cube::rotateZ(float angle)
{
	// Modify the coordinates of teh cube's center.
	float xNew = (vCenter[0] - vOrigin[0])*std::cos(angle) - 
				(vCenter[1] - vOrigin[1])*std::sin(angle);
	float yNew = (vCenter[0] - vOrigin[0])*std::sin(angle) + 
		        (vCenter[1] - vOrigin[1])*std::cos(angle);

	vCenter[0] = xNew + vOrigin[0];
	vCenter[1] = yNew + vOrigin[1];
}

void Cube::moveX(int factor)
{
	vOrigin[0] += factor*size;
	vCenter[0] += factor*size;
}

void Cube::moveY()
{
	vOrigin[1] -= size;
	vCenter[1] -= size;
}

void Cube::moveZ(int factor)
{
	vOrigin[2] += factor*size;
	vCenter[2] += factor*size;
}

void Cube::setOrigin(vector_3d vO)
{
	vOrigin = vO;
}

const vector_3d& Cube::getCenter()
{
	return vCenter;
}

vector_3d Cube::testRotateX(float angle)
{
	vector_3d testCenter;
	// Modify the coordinates of teh cube's center.
	float yNew = (vCenter[1] - vOrigin[1])*std::cos(angle) - 
				(vCenter[2] - vOrigin[2])*std::sin(angle);
	float zNew = (vCenter[1] - vOrigin[1])*std::sin(angle) + 
				(vCenter[2] - vOrigin[2])*std::cos(angle);

	testCenter[0] = vCenter[0];
	testCenter[1] = yNew + vOrigin[1];
	testCenter[2] = zNew + vOrigin[2];

	return testCenter;
}

vector_3d Cube::testRotateY(float angle)
{
	vector_3d testCenter;

	// Modify the coordinates of teh cube's center.
	float xNew = (vCenter[0] - vOrigin[0])*std::cos(angle) - 
				(vCenter[2] - vOrigin[2])*std::sin(angle);
	float zNew = (vCenter[0] - vOrigin[0])*std::sin(angle) + 
				(vCenter[2] - vOrigin[2])*std::cos(angle);

	testCenter[1] = vCenter[1];
	testCenter[0] = xNew + vOrigin[0];
	testCenter[2] = zNew + vOrigin[2];

	return testCenter;
}

vector_3d Cube::testRotateZ(float angle)
{
	vector_3d testCenter;

	// Modify the coordinates of teh cube's center.
	float xNew = (vCenter[0] - vOrigin[0])*std::cos(angle) - 
				(vCenter[1] - vOrigin[1])*std::sin(angle);
	float yNew = (vCenter[0] - vOrigin[0])*std::sin(angle) + 
		        (vCenter[1] - vOrigin[1])*std::cos(angle);

	testCenter[0] = xNew + vOrigin[0];
	testCenter[1] = yNew + vOrigin[1];
	testCenter[2] = vCenter[2];

	return testCenter;
}

vector_3d Cube::testMoveX(int factor)
{
	vector_3d testCenter;
	testCenter = vCenter;	
	testCenter[0] += factor*size;

	return testCenter;
}

vector_3d Cube::testMoveZ(int factor)
{
	vector_3d testCenter;
	testCenter = vCenter;	
	testCenter[2] += factor*size;

	return testCenter;
}