#ifndef TRANSLATIONMATRIX_H
#define TRANSLATIONMATRIX_H


#include "../Math/Matrix.h"

class TranslationMatrix : public Matrix {
public:
    TranslationMatrix(double shiftX, double shiftY, double shiftZ) {
        matrix = {{1, 0, 0, shiftX},
                  {0, 1, 0, shiftY},
                  {0, 0, 1, shiftZ},
                  {0, 0, 0, 1}};
    }
};


#endif //TRANSLATIONMATRIX_H