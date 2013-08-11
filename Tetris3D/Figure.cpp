// Figure.cpp - implementation of Figure abstract class and its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Figure.h"

Figure::Figure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
	iNumOfCubes(nCubes), vOrigin(vO), id(idExt), cubeSize(size)
{
	//angleX = 0;
	//angleY = 0;
	//angleZ = 0;
	pi = 4*std::atan(1.);
	deltaAngleRad = 0.5*pi;
}

Figure::~Figure(void)
{
}

void Figure::draw()
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->draw();
}

void Figure::moveX(int factor)
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->moveX(factor);
}

void Figure::moveY()
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->moveY();
}

void Figure::moveZ(int factor)
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->moveZ(factor);
}

void Figure::rotateX()
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->rotateX(deltaAngleRad);
}

void Figure::rotateY()
{
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->rotateY(deltaAngleRad);
}

void Figure::rotateZ()
{
	//angleZ = (angleZ + 90) % 360;
	//float angleRad =  pi*double(angleZ)/180.;

	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->rotateZ(deltaAngleRad);
}

//_____________________//
// Lfigure.
Lfigure::Lfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
Figure(nCubes, vO, idExt, size)
{
	createCubes();
}

Lfigure::~Lfigure(void)
{
}

void Lfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	// Head cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[1] -= cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Two cubes below.
	++id;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	++id;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// The cube to the right.
	++id;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Ofigure.
Ofigure::Ofigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
Figure(nCubes, vO, idExt, size)
{
	createCubes();
}

Ofigure::~Ofigure(void)
{
}

void Ofigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[0] -= 0.5*cubeSize;	// Put the origin in the middle of the figure.
	center[1] += 0.5*cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left bottom.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right bottom.
	++id;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right top.
	++id;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Sfigure.
Sfigure::Sfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
Figure(nCubes, vO, idExt, size)
{
	createCubes();
}

Sfigure::~Sfigure(void)
{
}

void Sfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[0] -= 0.5*cubeSize;	// Put the origin in the middle of the figure.
	center[1] += cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left middle.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right middle.
	++id;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right bottom.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Ifigure.
Ifigure::Ifigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
Figure(nCubes, vO, idExt, size)
{
	createCubes();
}

Ifigure::~Ifigure(void)
{
}

void Ifigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[1] += 1.5*cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Bottom.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Tfigure.
Tfigure::Tfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size) :
Figure(nCubes, vO, idExt, size)
{
	createCubes();
}

Tfigure::~Tfigure(void)
{
}

void Tfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[1] += 0.5*cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left.
	++id;
	center[0] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right.
	++id;
	center[0] += 2.*cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(id, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}