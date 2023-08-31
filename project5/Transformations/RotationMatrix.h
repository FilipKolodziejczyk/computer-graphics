#ifndef ROTATIONMATRIX_H
#define ROTATIONMATRIX_H

#include "../Math/Matrix.h"

class RotationMatrix : public Matrix {
public:
    RotationMatrix(double angleX, double angleY, double angleZ) {
        double cosX = cos(angleX);
        double sinX = sin(angleX);
        double cosY = cos(angleY);
        double sinY = sin(angleY);
        double cosZ = cos(angleZ);
        double sinZ = sin(angleZ);

        auto matrixX = Matrix({{1, 0,    0,     0},
                               {0, cosX, -sinX, 0},
                               {0, sinX, cosX,  0},
                               {0, 0,    0,     1}});
        auto matrixY = Matrix({{cosY,  0, sinY, 0},
                               {0,     1, 0,    0},
                               {-sinY, 0, cosY, 0},
                               {0,     0, 0,    1}});
        auto matrixZ = Matrix({{cosZ, -sinZ, 0, 0},
                               {sinZ, cosZ,  0, 0},
                               {0,    0,     1, 0},
                               {0,    0,     0, 1}});

        matrix = (matrixX * matrixY * matrixZ).matrix;
    }
};

#endif //ROTATIONMATRIX_H