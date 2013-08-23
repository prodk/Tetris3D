// Figure.h - declaration of Figure abstract class and its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef FIGURE_H
#define FIGURE_H
#include "Cube.h"

class PlaneOfCells;

typedef std::vector<std::tr1::shared_ptr<PlaneOfCells> > pPlanes;

class Figure
{
public:
	Figure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size, int x, int p, int z);
	virtual ~Figure(void);

	// Functions to override.
	virtual void draw();
	virtual void drawAsNext();				// Is used when the figure is the next figure.
	virtual void rotateX(pPlanes& planes);
	virtual void rotateY(pPlanes& planes);
	virtual void rotateZ(pPlanes& planes);

	// Functions with the fixed implementation.
	void moveX(int factor, pPlanes& planes);
	void moveY(int factor, pPlanes& planes);
	void moveZ(int factor, pPlanes& planes);

	// Prevent from movement/rotation that results in a cube outside the scene.
	bool testMoveX(int factor, pPlanes& planes);// True if the figure doesn't move outside the scene.
	bool testMoveY(int factor, pPlanes& planes);// True if the figure doesn't move outside the scene.
	bool testMoveZ(int factor, pPlanes& planes);// True if the figure doesn't move outside the scene.

	bool testRotateX(pPlanes& planes);
	bool testRotateY(pPlanes& planes);
	bool testRotateZ(pPlanes& planes);

	void getCubeIndeces(std::vector<CellIndeces> &id);// Gets indeces of all the cubes of the Figure.
	const std::vector<std::tr1::shared_ptr<Cube> > & getCubes();

protected:
	virtual void createCubes() = 0;

protected:
	// External input.
	vector_3d vOrigin;	// The origin of the figure, typically the center of the cube #0.
	std::size_t iNumOfCubes;
	std::size_t figureId;
	float cubeSize;
	int iNumOfCellsX;
	int iNumOfPlanes;
	int iNumOfCellsZ;

	// Inner info.
	std::vector<std::tr1::shared_ptr<Cube> > cubes;
	float deltaAngleRad;// 90 degrees in radians.

	float pi;
};

//----------------------------------
// Lfigure.
class Lfigure : public Figure
{
public:
	Lfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
		float size, int x, int p, int z);
	virtual ~Lfigure(void);

protected:
	void createCubes();
};

//----------------------------------
// Ofigure.
class Ofigure : public Figure
{
public:
	Ofigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
		float size, int x, int p, int z);
	virtual ~Ofigure(void);

protected:
	void createCubes();
};

//----------------------------------
// Sfigure.
class Sfigure : public Figure
{
public:
	Sfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
		float size, int x, int p, int z);
	virtual ~Sfigure(void);

protected:
	void createCubes();
};

//----------------------------------
// Ifigure.
class Ifigure : public Figure
{
public:
	Ifigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
		float size, int x, int p, int z);
	virtual ~Ifigure(void);

protected:
	void createCubes();
};

//----------------------------------
// Tfigure.
class Tfigure : public Figure
{
public:
	Tfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, 
		float size, int x, int p, int z);
	virtual ~Tfigure(void);

protected:
	void createCubes();
};

#endif // FIGURE_H