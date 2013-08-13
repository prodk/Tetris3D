// FixedCubes.h - declarations of Cell, Plane and  FixedCubes classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef FIXED_CUBES_H
#define FIXED_CUBES_H

#include "Cube.h"

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

	// Draw mesh functions.
	void drawLeftFace(int x, int planeId, int z);
	void drawRightFace();
	void drawBottomFace();

private:
	//int x;					// Lateral index.
	//int z;					// Lateral index.
	//int planeId;			// In what plane the cell is located.
	int cubeId;				// An id of the fixed cube, -1 otherwise.

	float cubeSize;			// Required for drawing.
};

//--------------------------------
// PlaneOfCells.
// Manages cells in a vertical plane.
class PlaneOfCells
{
public:
	PlaneOfCells(int id, int x, int z, float size);
	~PlaneOfCells();

	bool isFilled();		// True if all the cells in the plane contain a fixed cube.
	void fillCell(int x, int z, int cubeId);// Mark the cell as containing a fixed cube.

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

private:
	void createPlanes();
	void annihilateLayer();

private:
	int iNumOfCubes;				// Current number of cubes.
	int iNumOfPlanes;
	int iNumOfCellsX;
	int iNumOfCellsZ;

	float cubeSize;

	std::vector<std::tr1::shared_ptr<PlaneOfCells> > plane;		// Vector of planes containing cells.

	std::vector<std::tr1::shared_ptr<Cube> > cubes;	// Pointers to the fixed cubes.
};

#endif	// end FIXED_CUBES_H