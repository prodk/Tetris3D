// FixedCubes.cpp - implementations of Cell, Plane and  FixedCubes classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "FixedCubes.h"

//----------------------------
// Cell.
Cell::Cell()
{
	cubeId = -1;
	cubeSize = 1.;
	polygonMode = GL_LINE;

	// Default material.
	pMaterialGrid = std::tr1::shared_ptr<Material>(new Material());
	pMaterialFill = std::tr1::shared_ptr<Material>(new Material());
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

	if(polygonMode == GL_LINE)
		pMaterialGrid->setValues();
	else 
		pMaterialFill->setValues();

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	
	glTranslatef(x*cubeSize, planeId*cubeSize, z*cubeSize);	// Translate to the corresp cell.
	// Draw a square in the yz-plane.
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
	
	glPopMatrix();
}

void Cell::drawRightFace(int x, int planeId, int z)
{
	glPushMatrix();

	if(polygonMode == GL_LINE)
		pMaterialGrid->setValues();
	else 
		pMaterialFill->setValues();

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	
	glTranslatef(x*cubeSize, planeId*cubeSize, z*cubeSize);	// Translate to the corresp cell.
	// Draw a square in the yz-plane.
	glBegin(GL_POLYGON);

	glNormal3f(1., 0., 0.);
    glVertex3f(cubeSize, 0.0, 0.0);    

	glNormal3f(1., 0., 0.);
    glVertex3f(cubeSize, 0.0, cubeSize);
    	
	glNormal3f(1., 0., 0.);
    glVertex3f(cubeSize, cubeSize, cubeSize);

	glNormal3f(1., 0., 0.);
	glVertex3f(cubeSize, cubeSize, 0.0);
	
	glEnd();

	glPopMatrix();
}

void Cell::drawBottomFace(int x, int planeId, int z)
{
	glPushMatrix();

	if(polygonMode == GL_LINE)
		pMaterialGrid->setValues();
	else 
		pMaterialFill->setValues();

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	
	glTranslatef(x*cubeSize, planeId*cubeSize, z*cubeSize);	// Translate to the corresp cell.
	// Draw a square in the xz-plane.
	glBegin(GL_POLYGON);

	glNormal3f(0., -1., 0.);
    glVertex3f(0.0, 0.0, 0.0);    

	glNormal3f(0., -1., 0.);
    glVertex3f(0., 0.0, cubeSize);
    	
	glNormal3f(0., -1., 0.);
    glVertex3f(cubeSize, 0., cubeSize);

	glNormal3f(0., -1., 0.);
	glVertex3f(cubeSize, 0., 0.0);
	
	glEnd();

	glPopMatrix();
}

void Cell::setPolygonMode(int mode)
{
	polygonMode = mode;
}

int Cell::getPolygonMode()
{
	return polygonMode;
}

void Cell::setMaterials(std::tr1::shared_ptr<Material> grid, std::tr1::shared_ptr<Material> fill)
{
	pMaterialGrid = grid;
	pMaterialFill = fill;
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
	// Set cell's material to draw.
	// Grid material.
	vector_3d ambient = vector_3d(0.0, 10.0, 0.0);
	vector_3d diffuse = vector_3d(0.0, 10.0, 0.0);
	vector_3d specular = vector_3d(0.0, 10.0, 0.0); 
	float shine = 50;
	float alpha = 0.8;

	std::tr1::shared_ptr<Material> pMaterialGrid = 
		std::tr1::shared_ptr<Material>(new Material(ambient, diffuse, specular, shine, alpha) );

	// Filling material.
	ambient = vector_3d(0.0, 5.0, 0.0);
	diffuse = vector_3d(0.0, 5.0, 0.0);
	specular = vector_3d(0.0, 5.0, 0.0); 
	shine = 50;
	alpha = 0.8;
	std::tr1::shared_ptr<Material> pMaterialFill = 
		std::tr1::shared_ptr<Material>(new Material(ambient, diffuse, specular, shine, alpha) );

	// Reset all the cube indexes to -1.
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){	// z-direction is the fast-scan direction.
			cell[ix*iNumOfCellsX + iz].setCubeSize(cubeSize);
			cell[ix*iNumOfCellsX + iz].setMaterials(pMaterialGrid, pMaterialFill);
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

bool PlaneOfCells::isEmpty()			// True if all the cells in the plane contain -1.
{
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){			
			if( cell[ix*iNumOfCellsX + iz].getCubeIdx() >= 0 )
				return false;
		}

	return true;
}

bool PlaneOfCells::isCellFilled(int x, int z)
{
	if( cell[x*iNumOfCellsX + z].getCubeIdx() < 0 )
		return false;

	return true;
}

void PlaneOfCells::fillCell(int x, int z, int cubeId)// Mark the cell as containing a fixed cube.
{
	cell[x*iNumOfCellsX + z].setCubeIdx(cubeId);
}

void PlaneOfCells::drawLeftFaces()
{
	int ix = 0;
	for(int iz = 0; iz < iNumOfCellsZ; iz++)
		cell[ix*iNumOfCellsX + iz].drawLeftFace(ix, planeId, iz);
}

void PlaneOfCells::drawBottomFaces()
{
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){			
			cell[ix*iNumOfCellsX + iz].drawBottomFace(ix, planeId, iz);
		}
}

void PlaneOfCells::drawRightFaces()
{
	int ix = iNumOfCellsX - 1;
	for(int iz = 0; iz < iNumOfCellsZ; iz++)
		cell[ix*iNumOfCellsX + iz].drawRightFace(ix, planeId, iz);
}

void PlaneOfCells::drawHighlightedBottomCells()
{
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){
			if( cell[ix*iNumOfCellsX + iz].getPolygonMode() == GL_FILL )
				cell[ix*iNumOfCellsX + iz].drawBottomFace(ix, planeId, iz);
		}
}

void PlaneOfCells::setHighlightedLeftCell(int z)
{
	int ix = 0;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_FILL);
}

void PlaneOfCells::resetHighlightedLeftCell(int z)
{
	int ix = 0;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_LINE);
}

void PlaneOfCells::setHighlightedRightCell(int z)
{
	int ix = iNumOfCellsX - 1;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_FILL);
}

void PlaneOfCells::resetHighlightedRightCell(int z)
{
	int ix = iNumOfCellsX - 1;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_LINE);
}

void PlaneOfCells::setHighlightedCell(int x, int z)
{
	cell[x*iNumOfCellsX + z].setPolygonMode(GL_FILL);	
}

void PlaneOfCells::resetHighlightedCell(int x, int z)
{
	cell[x*iNumOfCellsX + z].setPolygonMode(GL_LINE);	
}

int PlaneOfCells::getCubeIndex(int x, int z)
{
	return cell[x*iNumOfCellsX + z].getCubeIdx();
}

const std::vector<Cell>& PlaneOfCells::getCells()
{
	return cell;
}

void PlaneOfCells::copyCells(const std::vector<Cell>& c)
{
	for(std::size_t i = 0; i < c.size(); ++i)
		cell[i] = c[i];
}

//----------------------------
// FixedCubes.
FixedCubes::FixedCubes(int planes, int x, int z, float size)
{
	iNumOfCubes = 0;			// No fixed cubes at the beginning.
	cubeSize = size;
}

FixedCubes::~FixedCubes(void)
{
}

void FixedCubes::draw()
{
	// Draw fixed cubes.
	for(map_iter iterator = cubes.begin(); iterator != cubes.end(); iterator++)
		iterator->second->draw();
}

void FixedCubes::eraseCube(int cubeIndex)
{
	cubes.erase(cubeIndex);
}

void FixedCubes::moveCubeY(int factor, int cubeIndex)
{
	cubes[cubeIndex]->moveY(factor);
}

void FixedCubes::insertCube(std::tr1::shared_ptr<Cube> c, int cubeId)
{
	cubes.insert(std::make_pair(cubeId, c));	
}