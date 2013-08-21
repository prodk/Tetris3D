// Cube.h - declaration of Cube class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(std::size_t idExt, vector_3d c, float size);
	~Cube(void);

	void draw();
	float getSize() const;

	void rotateX(float angle);	// Rotate in the yz-plane, around x-axis.
	void rotateY(float angle);	// Rotate in the xz-plane, around y-axis.
	void rotateZ(float angle);	// Rotate in the xy-plane, around z-axis.

	// Emulate rotation of the cube to check the boundaries. Look at the center of the cube.
	vector_3d testRotateX(float angle);	
	vector_3d testRotateY(float angle);
	vector_3d testRotateZ(float angle);

	void moveX(int factor);
	void moveY(int factor);				// Fall.
	void moveZ(int factor);

	// Emulate the movement of the cube to check the boundaries.
	vector_3d testMoveX(int factor);
	vector_3d testMoveY(int factor);
	vector_3d testMoveZ(int factor);

	void setOrigin(vector_3d vO);

	const vector_3d& getCenter();

protected:
	vector_3d vOrigin;	// The origin of the coordinate system.
						// The center of the Cube vCenter is inherited from Shape.
						// It is counted from the origin, so in rotation the difference
						// between the center and the origin is used as the distance.
	float size;
};

// Helper structure for cell indeces.
struct CellIndeces
{
	CellIndeces()
	{
		x = plane = z = -1;
	}

	int x;
	int plane;
	int z;
};

#endif // CUBE_H