#ifndef Matrix4_H
#define Matrix4_H


#include "Vec3.h"

class Matrix4
{
public:
    // Constructor to initialize the matrix
    Matrix4(float data[16]) {
        for (int i = 0; i < 16; ++i) {
            m[i] = data[i];
        }
    }

    Matrix4() {};

    // Matrix-vector multiplication
    Vec3 multiplyVector(const Vec3& vec) const {
        float x = m[0] * vec.x() + m[4] * vec.y() + m[8] * vec.z() + m[12];
        float y = m[1] * vec.x() + m[5] * vec.y() + m[9] * vec.z() + m[13];
        float z = m[2] * vec.x() + m[6] * vec.y() + m[10] * vec.z() + m[14];
        return Vec3(x, y, z);
    }

    void calculateMatrix(int rotX, int rotY, int rotZ);
    void swapRows(int rowA, int rowB);
    void rowReduce(int row, float factor);

    void invert();

private:
    float m[16];
};



#endif // Matrix4_H
