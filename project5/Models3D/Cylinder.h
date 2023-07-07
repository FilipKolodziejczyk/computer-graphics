#ifndef CYLINDER_H
#define CYLINDER_H

#include "Mesh.h"

class Cylinder : public Mesh {
public:
    Cylinder(double radius, double height, int sides, Vector3 positionOnScen);

    double radius;
    double height;
};


#endif //CYLINDER_H