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

	//glDisable(GL_LIGHTING);
	//glColor3f(0., 1., 0.);
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

	glDisable( GL_TEXTURE_2D );
	//glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Cell::drawRightFace(int x, int planeId, int z)
{
	glPushMatrix();

	//glDisable(GL_LIGHTING);
	//glColor3f(0., 1., 0.);
	//pMaterial->setValues();
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

	glDisable( GL_TEXTURE_2D );
	//glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Cell::drawBottomFace(int x, int planeId, int z)
{
	glPushMatrix();

	//glDisable(GL_LIGHTING);
	//glColor3f(0., 1., 0.);
	//pMaterial->setValues();
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

	glDisable( GL_TEXTURE_2D );
	//glEnable(GL_LIGHTING);
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
	float alpha = 0.7;

	std::tr1::shared_ptr<Material> pMaterialGrid = 
		std::tr1::shared_ptr<Material>(new Material(ambient, diffuse, specular, shine, alpha) );

	// Filling material.
	ambient = vector_3d(0.0, 5.0, 0.0);
	diffuse = vector_3d(0.0, 5.0, 0.0);
	specular = vector_3d(0.0, 5.0, 0.0); 
	shine = 50;
	alpha = 0.9;
	std::tr1::shared_ptr<Material> pMaterialFill = 
		std::tr1::shared_ptr<Material>(new Material(ambient, diffuse, specular, shine, alpha) );

	// Reset all the cube indexes to -1.
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){	// z-direction is the fast-scan direction.
			cell[ix*iNumOfCellsX + iz].setCubeSize(cubeSize);
			cell[ix*iNumOfCellsX + iz].setMaterials(pMaterialGrid, pMaterialFill);
		}
}

bool PlaneOfCells::isPlaneFilled()			// True if all the cells in the plane contain a fixed cube.
{
	// The plane is filled when there're no indexes which are == -1.
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){			
			if( cell[ix*iNumOfCellsX + iz].getCubeIdx() < 0 )
				return false;
		}

	return true;
}

bool PlaneOfCells::isPlaneEmpty()			// True if all the cells in the plane contain -1.
{
	// The plane is filled when there're no indexes which are == -1.
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

void PlaneOfCells::drawLeftPlane()
{
	int ix = 0;
	for(int iz = 0; iz < iNumOfCellsZ; iz++)
		cell[ix*iNumOfCellsX + iz].drawLeftFace(ix, planeId, iz);
}

void PlaneOfCells::drawBottomPlane()
{
	for(int ix = 0; ix < iNumOfCellsX; ix++)
		for(int iz = 0; iz < iNumOfCellsZ; iz++){			
			cell[ix*iNumOfCellsX + iz].drawBottomFace(ix, planeId, iz);
		}
}

void PlaneOfCells::drawRightPlane()
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

void PlaneOfCells::setFilledCellToDraw(int x, int z)
{
	// Set the leftmost plane.
	int ix = 0;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_FILL);

	// Set the rightmost plane.
	ix = iNumOfCellsX - 1;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_FILL);
}

void PlaneOfCells::resetFilledCellToDraw(int x, int z)
{
	//cell[x*iNumOfCellsX + z].setPolygonMode(GL_LINE);
	// Reset the leftmost plane.
	int ix = 0;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_LINE);

	// Reset the rightmost plane.
	ix = iNumOfCellsX - 1;
	cell[ix*iNumOfCellsX + z].setPolygonMode(GL_LINE);
}

void PlaneOfCells::setBottomCellToDraw(int x, int z)
{
	cell[x*iNumOfCellsX + z].setPolygonMode(GL_FILL);	
}

void PlaneOfCells::resetBottomCellToDraw(int x, int z)
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
	iNumOfPlanes = planes;
	iNumOfCellsX = x;
	iNumOfCellsZ = z;
	cubeSize = size;

	//iFiguresPerCube = 4;

	bottomPlane = 0;		// Plane on which we highlight cells.
	secondBottomPlane = 0;

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

void FixedCubes::draw()
{
	glPushMatrix();

	// Translate the coordinates, such that the origin is at the center of the scene.
	glTranslatef(-0.5*cubeSize*iNumOfCellsX, -0.5*cubeSize*iNumOfPlanes, -0.5*cubeSize*iNumOfCellsZ);

	for(std::size_t i = 0; i < plane.size(); ++i){
		plane[i]->drawLeftPlane();
		plane[i]->drawRightPlane();
	}

	plane[bottomPlane]->drawBottomPlane();
	plane[secondBottomPlane]->drawHighlightedBottomCells();

	glPopMatrix();

	// Draw fixed cubes.
	for(map_iter iterator = cubes.begin(); iterator != cubes.end(); iterator++)
		iterator->second->draw();
}

void FixedCubes::annihilateLayer()
{
	int index;
	int emptyValue = -1;
	std::size_t i = 0;
	while(i < plane.size()){
		if( plane[i]->isPlaneFilled() ){
			// Annihilate the plane.
			// Get indeces of the cubes in this plane and delete these cubes.
			for(int ix = 0; ix < iNumOfCellsX; ix++){
				for(int iz = 0; iz < iNumOfCellsZ; iz++){
					index = plane[i]->getCubeIndex(ix, iz);
					// Remove the cube
					cubes.erase(index);
					// Say that the cell is empty.
					plane[i]->fillCell(ix, iz, emptyValue);
#ifdef _DEBUG
					std::cerr << "Plane " << i << " has been annihilated." << std::endl;
#endif
				}
			}// End annihilate the plane.	

			// Find the first empty plane above the annihilated plane.
			std::size_t emptyPlaneIndex = 0;
			for(std::size_t j = i+1; j < plane.size(); ++j){
				if( plane[j]->isPlaneEmpty() ){
					emptyPlaneIndex = j;
#ifdef _DEBUG
					std::cerr << "Empty plane " << emptyPlaneIndex << std::endl;
#endif
					goto label;	// Use goto here. break can terminate the outer while loop too. We don't want this.
				}
			}
label:

			// Shift all the nonempty planes by one plane to the bottom.
			// This involves 2 steps:
			// 1) copy the content of the cells of the upper plane to the current one.
			// 2) shift the corresponding cubes; 			
			for(std::size_t j = i; j < emptyPlaneIndex; ++j){
				plane[j]->copyCells( plane[j+1]->getCells() );// 1) copy cells of the upper plane to the current one.
#ifdef _DEBUG
				std::cerr << "plane " << j+1 << " moved to " << j << std::endl;
#endif
				// 2) shift the corresponding cubes.
				for(int ix = 0; ix < iNumOfCellsX; ix++){
					for(int iz = 0; iz < iNumOfCellsZ; iz++){
						index = plane[j]->getCubeIndex(ix, iz);
						// If the cell is not empty, move the cube by one cube size to the bottom.
						if(index >= 0)
							cubes[index]->moveY();						
					}
				}// End shift the cubes.	
			}// End shift nonempty planes.

			--secondBottomPlane;	// Diminish the second highlighted plane.

			// Don't increase count i, start the 'while' loop again from the current plane i, 
			// which has just been shifted.
			
		}// End if plane is filled.
		++i;	// Go to the next upper plane if no annihilation.
	}
}

void FixedCubes::setFilledCellToDraw(CellIndeces &id)
{
	plane[id.plane]->setFilledCellToDraw(id.x, id.z);
}

void FixedCubes::resetFilledCellToDraw(CellIndeces &id)
{
	plane[id.plane]->resetFilledCellToDraw(id.x, id.z);
}

void FixedCubes::setBottomCellToDraw(CellIndeces &id)
{
	plane[bottomPlane]->setBottomCellToDraw(id.x, id.z);
	plane[secondBottomPlane]->setBottomCellToDraw(id.x, id.z);
}

void FixedCubes::resetBottomCellToDraw(CellIndeces &id)
{
	plane[bottomPlane]->resetBottomCellToDraw(id.x, id.z);
	plane[secondBottomPlane]->resetBottomCellToDraw(id.x, id.z);
}

bool FixedCubes::isCellFilled(int x, int planeId, int z)
{
	return plane[planeId]->isCellFilled(x, z);
}

void FixedCubes::addCubes(Figure &f)
{
	std::vector<CellIndeces> ci;
	ci.resize(4);

	std::vector<std::tr1::shared_ptr<Cube> > c;

	f.getCubeIndeces(ci);
	c = f.getCubes();
	// Add new cubes to the cubes and mark cells that contain these cubes.
	for(std::size_t i = 0; i < c.size(); i++){
		// cubes.push_back(c[i]);
		int cubeId = c[i]->getId();
//#ifdef _DEBUG
//		std::cerr << cubeId << std::endl;
//#endif
		cubes.insert(std::make_pair(cubeId, c[i]));	
		// Notify the cell in the plane that the cell is filled with the fixed cube.
		// Use cube id as an id in the function below.
		fillCell(ci[i], cubeId );	// Reconsider the cube.size() as id when annihilation.
	}

	// Find the topmost nonempty plane.
	for(std::size_t i = 0; i < plane.size(); ++i)
		if( plane[i]->isPlaneEmpty() )
			break;
		else
			secondBottomPlane = i;
}

void FixedCubes::fillCell(const CellIndeces &cellId, int cubeId)
{
	plane[cellId.plane]->fillCell(cellId.x, cellId.z, cubeId);
}