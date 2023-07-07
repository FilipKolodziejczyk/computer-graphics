#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H


#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix.h"
#include "./TranslationMatrix.h"
#include "RotationMatrix.h"
#include "ScaleMatrix.h"

class Transformation {
public:
    Transformation(Vector3 srcOrigin, Vector3 dstOrigin, Vector3 rotation, Vector3 scale) : _localPosition(srcOrigin),
                                                                                            _position(dstOrigin),
                                                                                            _rotation(rotation),
                                                                                            _scale(scale) {}

    Transformation() {}

    Vector4 transform(const Vector4 &vector) const {
        auto translationMatrix = TranslationMatrix(_position.x, _position.y, _position.z);
        auto rotationMatrix = RotationMatrix(_rotation.x, _rotation.y, _rotation.z);
        auto scaleMatrix = ScaleMatrix(_scale.x, _scale.y, _scale.z);

        auto temp = translationMatrix.dot(rotationMatrix);
        temp = temp.dot(scaleMatrix);
        temp = temp.dot(Matrix(vector));
        return temp.toVector4();

        // return translationMatrix.dot(rotationMatrix).dot(scaleMatrix).dot(Matrix(vector)).toVector4();
    }

    Vector3 _localPosition = Vector3(0, 0, 0);
    Vector3 _position = Vector3(0, 0, 0);
    Vector3 _rotation = Vector3(0, 0, 0);
    Vector3 _scale = Vector3(1, 1, 1);
};


#endif //TRANSFORMATION_H