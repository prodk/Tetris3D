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
	void moveX(int factor);
	void moveY();		// Fall with the constant velocity.
	void moveZ(int factor);
	void setOrigin(vector_3d vO);

protected:
	vector_3d vOrigin;
	float size;
};

#endif // CUBE_H