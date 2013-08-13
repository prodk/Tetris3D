// FixedCubes.cpp - implementations of Cell, Plane and  FixedCubes classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "FixedCubes.h"

//----------------------------
// Cell.
//Cell::Cell(int xExt, int zExt, int plane, float cube):
//	x(xExt), z(zExt), planeId(plane), cubeSize(cube)
//{
//}

Cell::Cell()
{
	cubeId = -1;
	cubeSize = 1.;
}

Cell::~Cell()
{
}

void Cell::setCubeIdx(int idx)
{
	cubeId = idx;
}

void Cell::setCubeSize(float size)
{
	cubeSize = size;
}

int Cell::getCubeIdx()
{
	return cubeId;
}

void Cell::drawLeftFace(int x, int planeId, int z)
{
	glPushMatrix();

	glDisable(GL_LIGHTING);
	glColor3f(0., 1., 0.);
	
	glTranslatef(x*cubeSize, planeId*cubeSize, z*cubeSize);	// Translate to the corresp cell.
	// Draw a square in the yz-plane.
	/*glBegin( GL_QUADS );
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0., 0.0, cubeSize);
	glVertex3d(0., cubeSize, cubeSize);
	glVertex3d(0.0, cubeSize, 0.0);	
	glEnd();*/

	glBegin(GL_POLYGON);

	glNormal3f(-1., 0., 0.);
    glVertex3f(0.0, 0.0, 0.0);    

	glNormal3f(-1., 0., 0.);
    glVertex3f(0., 0.0, cubeSize);
    	
	glNormal3f(-1., 0., 0.);
    glVertex3f(0., cubeSize, cubeSize);

	glNormal3f(-1., 0., 0.);
	glVertex3f(0.0, cubeSize, 0.0);
	
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Cell::drawRightFace()
{
}

void Cell::drawBottomFace()
{
}

//----------------------------
// PlaneOfCells.
PlaneOfCells::PlaneOfCells(int id, int x, int z, float size)
{
	planeId = id;
	iNumOfCellsX = x;
	iNumOfCellsZ = z;
	cubeSize = size;

	initCells();
}

PlaneOfCells::~PlaneOfCells()
{
}

void PlaneOfCells::initCells()
{
	cell.resize(iNumOfCellsX*iNumOfCellsZ);
	// Reset all the cube indexes to -1.
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){	// z-direction is the fast-scan direction.
			cell[ix*iNumOfCellsX + iz].setCubeSize(cubeSize);
		}
}

bool PlaneOfCells::isFilled()			// True if all the cells in the plane contain a fixed cube.
{
	// The plane is filled when there're no indexes which are == -1.
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){			
			if( cell[ix*iNumOfCellsX + iz].getCubeIdx() < 0 )
				return false;
		}

	return true;
}

void PlaneOfCells::fillCell(int x, int z, int cubeId)// Mark the cell as containing a fixed cube.
{
	cell[x*iNumOfCellsX + z].setCubeIdx(cubeId);
}

//----------------------------
// FixedCubes.
FixedCubes::FixedCubes(int planes, int x, int z, float size)
{
	iNumOfCubes = 0;			// No fixed cubes at the beginning.
	iNumOfPlanes = planes;
	iNumOfCellsX = x;
	iNumOfCellsZ = z;
	cubeSize = size;

	// Create the required number of planes.
	createPlanes();
}

FixedCubes::~FixedCubes(void)
{
}

void FixedCubes::createPlanes()
{
	plane.resize(iNumOfPlanes);

	for(std::size_t i = 0; i < plane.size(); i++)
	{
		plane[i] = 
			std::tr1::shared_ptr<PlaneOfCells>( new PlaneOfCells(i, iNumOfCellsX, iNumOfCellsZ, cubeSize) );
	}
}

void FixedCubes::annihilateLayer()
{
}
	