// Figure.h - declaration of Figure abstract class and its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef FIGURE_H
#define FIGURE_H
#include "Cube.h"

class Figure
{
public:
	Figure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Figure(void);

	// Functions to override.
	virtual void draw();
	virtual void rotateX();
	virtual void rotateY();
	virtual void rotateZ();

	// Functions with the fixed implementation.
	void moveX(int factor);
	void moveY();
	void moveZ(int factor);

protected:
	virtual void createCubes() = 0;

protected:
	// External input.
	vector_3d vOrigin;	// The origin of the figure, typically the center of the cube #0.
	std::size_t iNumOfCubes;
	std::size_t id;
	float cubeSize;

	// Inner info.
	std::vector<std::tr1::shared_ptr<Cube> > cubes;
	float deltaAngleRad;// 90 degrees in radians.

	float pi;
};

//___________________//
// Lfigure.
class Lfigure : public Figure
{
public:
	Lfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Lfigure(void);

protected:
	void createCubes();
};

//___________________//
// Ofigure.
class Ofigure : public Figure
{
public:
	Ofigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Ofigure(void);

protected:
	void createCubes();
};

//___________________//
// Sfigure.
class Sfigure : public Figure
{
public:
	Sfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Sfigure(void);

protected:
	void createCubes();
};

//___________________//
// Ifigure.
class Ifigure : public Figure
{
public:
	Ifigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Ifigure(void);

protected:
	void createCubes();
};

//___________________//
// Tfigure.
class Tfigure : public Figure
{
public:
	Tfigure(std::size_t nCubes, vector_3d vO, std::size_t idExt, float size);
	virtual ~Tfigure(void);

protected:
	void createCubes();
};

#endif // FIGURE_H