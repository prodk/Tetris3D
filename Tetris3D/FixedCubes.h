// FixedCubes.h - declarations of Cell, Plane and  FixedCubes classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef FIXED_CUBES_H
#define FIXED_CUBES_H

#include "Cube.h"
#include "Figure.h"
#include <map>

//--------------------------------
// Cell.
// Contains the relevant info about the presence of a cube at a particular point in space.
// Also is used to draw a mesh on the boundary planes.
class Cell
{
public:
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
// Manages cells in a plane parallel to the xz-plane.
class PlaneOfCells
{
public:
	PlaneOfCells(int id, int x, int z, float size);
	~PlaneOfCells();

	bool isFilled();				// True if all the cells in the plane contain a fixed cube.
	bool isEmpty();
	bool isCellFilled(int x, int z);// True if the cell[x*Nx + z] is filled.
	void fillCell(int x, int z, int cubeId);// Mark the cell as containing a fixed cube.
	void drawLeftFaces();
	void drawBottomFaces();
	void drawRightFaces();
	void drawHighlightedBottomCells();

	void setHighlightedLeftCell(int z);	// Indicate the cell to be highlighted.
	void resetHighlightedLeftCell(int z);// Say that the cell shouldn't be highlighted.
	void setHighlightedRightCell(int z);	// Indicate the cell to be highlighted.
	void resetHighlightedRightCell(int z);// Say that the cell shouldn't be highlighted.
	void setHighlightedCell(int x, int z); // Set any highlighted cell.
	void resetHighlightedCell(int x, int z);// Reset any highlighted cell.

	int getCubeIndex(int x, int z);				// Return the index of a cube stored in the cell.
	const std::vector<Cell>& getCells();		// Rerurn all the cells.
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
	void eraseCube(int cubeIndex);
	void moveCubeY(int factor, int cubeIndex);
	void insertCube(std::tr1::shared_ptr<Cube> c, int cubeId);

private:
	typedef std::map<std::size_t, std::tr1::shared_ptr<Cube>  >::iterator map_iter;
	int iNumOfCubes;				// Current number of cubes.
	float cubeSize;
	std::map<std::size_t, std::tr1::shared_ptr<Cube> > cubes;
};

#endif	// end FIXED_CUBES_H