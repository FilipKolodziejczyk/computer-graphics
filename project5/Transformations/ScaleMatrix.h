#ifndef SCALEMATRIX_H
#define SCALEMATRIX_H

#include "../Math/Matrix.h"

class ScaleMatrix : public Matrix {
public:
    ScaleMatrix(double scaleX, double scaleY, double scaleZ) {
        matrix = {
            {scaleX, 0, 0, 0},
            {0, scaleY, 0, 0},
            {0, 0, scaleZ, 0},
            {0, 0, 0, 1}
        };
    }
};

#endif //SCALEMATRIX_H