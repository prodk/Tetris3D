// Figure.cpp - implementation of Figure abstract class and its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Figure.h"
#include "FixedCubes.h"

Figure::Figure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
	iNumOfCubes(nCubes), vOrigin(vO), figureId(idExt), cubeSize(size),
		iNumOfCellsX(x), iNumOfPlanes(p), iNumOfCellsZ(z)
{
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

void Figure::drawAsNext()
{
	glPushMatrix();
	glTranslatef(-cubeSize*(iNumOfCellsX + 1), -3.*cubeSize, 0.0);
	for(std::size_t i = 0; i < cubes.size(); ++i)
		cubes[i]->draw();

	glPopMatrix();
}

void Figure::moveX(int factor, pPlanes& planes)
{
	// Move only if the figure doesn't go beyond the borders of the scene.
	if( testMoveX(factor, planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->moveX(factor);
	}
}

void Figure::moveY(int factor, pPlanes& planes)
{
	if( testMoveY(factor, planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->moveY(factor);
	}
}

void Figure::moveZ(int factor, pPlanes& planes)
{
	// Move only inside the scene.
	if( testMoveZ(factor, planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->moveZ(factor);
	}
}

bool Figure::testMoveX(int factor, pPlanes& planes)
{
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testMoveX(factor);
		n = (testCenter[0] + shiftX)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsX) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

bool Figure::testMoveY(int factor, pPlanes& planes)
{
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testMoveY(factor);
		n = (testCenter[1] + shiftY)/cubeSize;
		if( (n < 0) || (n >= iNumOfPlanes) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

bool Figure::testMoveZ(int factor, pPlanes& planes)
{
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testMoveZ(factor);
		n = (testCenter[2] + shiftZ)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsZ) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

void Figure::rotateX(pPlanes& planes)
{
	// Rotate only after ensuring that the figure will not leave the borders of the scene.
	if( testRotateX(planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->rotateX(deltaAngleRad);
	}
}

void Figure::rotateY(pPlanes& planes)
{
	// Rotate only after ensuring that the figure will not leave the borders of the scene.
	if( testRotateY(planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->rotateY(deltaAngleRad);
	}
}

void Figure::rotateZ(pPlanes& planes)
{
	// Rotate only after ensuring that the figure will not leave the borders of the scene.
	if( testRotateZ(planes) ){
		for(std::size_t i = 0; i < cubes.size(); ++i)
			cubes[i]->rotateZ(deltaAngleRad);
	}
}

bool Figure::testRotateX(pPlanes& planes)
{
	// For rotation around x-axis look at the z and y directions.
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testRotateX(deltaAngleRad);
		// y-direction.
		n = (testCenter[1] + shiftY)/cubeSize;
		if( (n < 0) || (n >= iNumOfPlanes) )
			return false;
		// z-direction.
		n = (testCenter[2] + shiftZ)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsZ) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

bool Figure::testRotateY(pPlanes& planes)
{
	// For rotation around y-axis look at the x- and z-directions;
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testRotateY(deltaAngleRad);
		// x-direction.
		n = (testCenter[0] + shiftX)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsX) )
			return false;
		// z-direction.
		n = (testCenter[2] + shiftZ)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsZ) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

bool Figure::testRotateZ(pPlanes& planes)
{
	// For rotation around z-axis look at the x- and y-directions;
	float n;
	vector_3d testCenter;
	int iCellX, iPlane, iCellZ;
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;

	for(std::size_t i = 0; i < cubes.size(); ++i){
		testCenter = cubes[i]->testRotateZ(deltaAngleRad);
		// x-direction.
		n = (testCenter[0] + shiftX)/cubeSize;
		if( (n < 0) || (n >= iNumOfCellsX) )
			return false;
		// z-direction.
		n = (testCenter[1] + shiftY)/cubeSize;
		if( (n < 0) || (n >= iNumOfPlanes) )
			return false;

		// Define cell indeces.
		iCellX = (testCenter[0] + shiftX)/cubeSize;
		iPlane = (testCenter[1] + shiftY)/cubeSize;
		iCellZ = (testCenter[2] + shiftZ)/cubeSize;
		if( planes[iPlane]->isCellFilled(iCellX, iCellZ) )
			return false;
	}

	return true;
}

void Figure::getCubeIndeces(std::vector<CellIndeces> &indeces)
{
	std::size_t iIdSize = indeces.size();
	float shiftX = 0.5*cubeSize*iNumOfCellsX;
	float shiftY = 0.5*cubeSize*iNumOfPlanes;
	float shiftZ = 0.5*cubeSize*iNumOfCellsZ;
	vector_3d center;

	for(std::size_t i = 0; i < cubes.size(); ++i)
		if(i < iIdSize )			// Treat only those cubes that fit the id array.
		{
			center = cubes[i]->getCenter();
			indeces[i].x = (center[0] + shiftX)/cubeSize;
			indeces[i].plane = (center[1] + shiftY)/cubeSize;
			indeces[i].z = (center[2] + shiftZ)/cubeSize;
		}
}

const std::vector<std::tr1::shared_ptr<Cube> > & Figure::getCubes()
{
	return cubes;
}

//_____________________//
// Lfigure.
Lfigure::Lfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
Figure(nCubes, vO, idExt, size, x, p, z)
{
	createCubes();
}

Lfigure::~Lfigure(void)
{
}

//!Put the duplicated code for cubes creation in a separate method!
void Lfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	std::size_t cubeIndex = figureId;
	// Head cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;

	// Put the rotation center to one of the vertices.
	center[0] += 0.5*cubeSize;
	center[1] -= 1.5*cubeSize;
	center[2] += 0.5*cubeSize;

	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Two cubes below.
	++id;
	++cubeIndex;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	++id;
	++cubeIndex;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// The cube to the right.
	++id;
	++cubeIndex;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Ofigure.
Ofigure::Ofigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
Figure(nCubes, vO, idExt, size, x, p, z)
{
	createCubes();
}

Ofigure::~Ofigure(void)
{
}

void Ofigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	std::size_t cubeIndex = figureId;
	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	center[0] -= 0.5*cubeSize;	// Put the origin in the middle of the figure.
	center[1] += 0.5*cubeSize;	// Put the origin in the middle of the figure.
	center[2] += 0.5*cubeSize;	// Put the origin in the middle of the figure.
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left bottom.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right bottom.
	++id;
	++cubeIndex;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right top.
	++id;
	++cubeIndex;
	center[1] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Sfigure.
Sfigure::Sfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
Figure(nCubes, vO, idExt, size, x, p, z)
{
	createCubes();
}

Sfigure::~Sfigure(void)
{
}

void Sfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	std::size_t cubeIndex = figureId;
	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;

	// Put the rotation point to one of the verteces.
	center[0] -= 0.5*cubeSize;
	center[1] += 0.5*cubeSize;
	center[2] += 0.5*cubeSize;

	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left middle.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right middle.
	++id;
	++cubeIndex;
	center[0] += cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right bottom.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Ifigure.
Ifigure::Ifigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
Figure(nCubes, vO, idExt, size, x, p, z)
{
	createCubes();
}

Ifigure::~Ifigure(void)
{
}

void Ifigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	std::size_t cubeIndex = figureId;
	// Left top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;


	// Put the rotation point to one of the verteces.
	center[0] += 0.5*cubeSize;
	center[1] += 1.5*cubeSize;
	center[2] += 0.5*cubeSize;

	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Bottom.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}

//_____________________//
// Tfigure.
Tfigure::Tfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
	float size, int x, int p, int z) :
Figure(nCubes, vO, idExt, size, x, p, z)
{
	createCubes();
}

Tfigure::~Tfigure(void)
{
}

void Tfigure::createCubes()
{
	cubes.resize(iNumOfCubes);

	std::size_t cubeIndex = figureId;
	// Top cube.
	std::size_t id = 0;
	vector_3d center = vOrigin;
	// Prevent rotational problems.
	center[0] += 0.5*cubeSize;	// Put the origin to a vertex.
	center[1] += 0.5*cubeSize;	// Put the origin to a vertex.
	center[2] += 0.5*cubeSize;	// Put the origin to a vertex.

	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Middle.
	++id;
	++cubeIndex;
	center[1] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Left.
	++id;
	++cubeIndex;
	center[0] -= cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);

	// Right.
	++id;
	++cubeIndex;
	center[0] += 2.*cubeSize;
	cubes[id] = std::tr1::shared_ptr<Cube>( new Cube(cubeIndex, center, cubeSize) );
	cubes[id]->setOrigin(vOrigin);
}