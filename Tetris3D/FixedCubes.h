// FixedCubes.h - declarations of Cell, Plane and  FixedCubes classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef FIXED_CUBES_H
#define FIXED_CUBES_H

#include "Cube.h"
#include "Figure.h"
#include <map>

//--------------------------------
// Cell.
// Containes the relevant info about the presence of a cube at a particular point in space.
// Also is used to draw a mesh on the boundary planes.
class Cell
{
public:
	//Cell(int xExt, int zExt, int plane, float cube);
	Cell();
	~Cell();

	void setCubeIdx(int idx);
	int getCubeIdx();

	void setCubeSize(float size);
	void setMaterials(std::tr1::shared_ptr<Material> grid, std::tr1::shared_ptr<Material> fill);

	// Draw mesh functions.
	void drawLeftFace(int x, int planeId, int z);
	void drawRightFace(int x, int planeId, int z);
	void drawBottomFace(int x, int planeId, int z);

	void setPolygonMode(int mode);
	int getPolygonMode();

private:
	int cubeId;				// An id of the fixed cube, -1 otherwise.
	int polygonMode;

	float cubeSize;			// Required for drawing.

	std::tr1::shared_ptr<Material> pMaterialGrid;
	std::tr1::shared_ptr<Material> pMaterialFill;
};

//--------------------------------
// PlaneOfCells.
// Manages cells in a vertical plane.
class PlaneOfCells
{
public:
	PlaneOfCells(int id, int x, int z, float size);
	~PlaneOfCells();

	bool isPlaneFilled();		// True if all the cells in the plane contain a fixed cube.
	bool isPlaneEmpty();
	bool isCellFilled(int x, int z);// True if the cell[x*Nx + z] is filled.
	void fillCell(int x, int z, int cubeId);// Mark the cell as containing a fixed cube.
	void drawLeftPlane();
	void drawBottomPlane();
	void drawRightPlane();
	void drawHighlightedBottomCells();

	void setFilledCellToDraw(int x, int z);
	void resetFilledCellToDraw(int x, int z);
	void setBottomCellToDraw(int x, int z);
	void resetBottomCellToDraw(int x, int z);

	//void clear();	// Delete cubes and empty the corresponding cells.
	int getCubeIndex(int x, int z);
	const std::vector<Cell>& getCells();
	void copyCells(const std::vector<Cell>& c);

private:
	void initCells();		// Set cube index in all the cells to -1.

private:
	int planeId;			// The number of the plane in the vertical (y) direction.
	int iNumOfCellsX;
	int iNumOfCellsZ;

	float cubeSize;

	std::vector<Cell> cell;	// Vector of cells in the plane.
};

//---------------------------------
// FixedCubes.
// A mediator class, manages collisions of the figure and annihilates layers of fixed cubes.
class FixedCubes
{
public:
	FixedCubes(int planes, int x, int z, float size);
	~FixedCubes(void);

	void draw();
	void collide();
	void setFilledCellToDraw(CellIndeces &id);
	void resetFilledCellToDraw(CellIndeces &id);
	void setBottomCellToDraw(CellIndeces &id);
	void resetBottomCellToDraw(CellIndeces &id);

	bool isCellFilled(int x, int planeId, int z);

	void addCubes(Figure &f);
	void fillCell(const CellIndeces &cellId, int cubeId);
	void annihilateLayer();

private:
	void createPlanes();
	//void annihilateLayer();

private:
	int iNumOfCubes;				// Current number of cubes.
	int iNumOfPlanes;
	int iNumOfCellsX;
	int iNumOfCellsZ;

	//int iFiguresPerCube;	// Add this to the constructor.

	float cubeSize;
	int bottomPlane;
	int secondBottomPlane;

	std::vector<std::tr1::shared_ptr<PlaneOfCells> > plane;	// Vector of planes containing cells.

	//std::vector<std::tr1::shared_ptr<Cube> > cubes;	// Pointers to the fixed cubes.
	std::map<std::size_t, std::tr1::shared_ptr<Cube> > cubes;
	typedef std::map<std::size_t, std::tr1::shared_ptr<Cube>  >::iterator map_iter;
};

#endif	// end FIXED_CUBES_H