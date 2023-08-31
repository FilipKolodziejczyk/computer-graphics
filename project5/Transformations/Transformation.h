#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H


#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "./TranslationMatrix.h"
#include "RotationMatrix.h"
#include "ScaleMatrix.h"

class Transformation {
public:
    Transformation(Vector src, Vector dst, Vector rotation, Vector scale) : src(src), dst(dst), rotation(rotation),
                                                                            scale(scale) {}

    Transformation() {}

    Vector transform(const Vector &vector) const {
        auto translationMatrix = TranslationMatrix(dst[0], dst[1], dst[2]);
        auto rotationMatrix = RotationMatrix(rotation[0], rotation[1], rotation[2]);
        auto scaleMatrix = ScaleMatrix(scale[0], scale[1], scale[2]);

        return (translationMatrix * rotationMatrix * scaleMatrix * Matrix(vector)).toVector();
    }

    Vector src = Vector(0, 0, 0);
    Vector dst = Vector(0, 0, 0);
    Vector rotation = Vector(0, 0, 0);
    Vector scale = Vector(1, 1, 1);
};


#endif //TRANSFORMATION_H