#include "Matrix4.h"

void Matrix4::calculateMatrix(int rotX, int rotY, int rotZ) {

    float rotRadX = rotX * (M_PI / 180.0);
    float rotRadY = rotY * (M_PI / 180.0);
    float rotRadZ = rotZ * (M_PI / 180.0);

    float cosX = cos(rotRadX);
    float sinX = sin(rotRadX);
    float cosY = cos(rotRadY);
    float sinY = sin(rotRadY);
    float cosZ = cos(rotRadZ);
    float sinZ = sin(rotRadZ);

    

    this->m[0] = cosY * cosZ;
    this->m[1] = -cosY * sinZ;
    this->m[2] = sinY;
    this->m[3] = 0.0;

    this->m[4] = sinX * sinY * cosZ + cosX * sinZ;
    this->m[5] = -sinX * sinY * sinZ + cosX * cosZ;
    this->m[6] = -sinX * cosY;
    this->m[7] = 0.0f;

    this->m[8] = -cosX * sinY * cosZ + sinX * sinZ;
    this->m[9] = cosX * sinY * sinZ + sinX * cosZ;
    this->m[10] = cosX * cosY;
    this->m[11] = 0.0f;

    this->m[12] = 0.0f;
    this->m[13] = 0.0f;
    this->m[14] = 0.0f;
    this->m[15] = 1.0f;
}

void Matrix4::swapRows(int rowA, int rowB){
    for (int i = 0; i < 4; i++) {
        std::swap(m[rowA * 4 + i], m[rowB * 4 + i]);
    }
}

void Matrix4::rowReduce(int row, float factor) {
    for (int i = 0; i < 4; i++) {
        m[row * 4 + i] /= factor;
    }
}


void Matrix4::invert() {

    //code taken from stack overflow, answer by user shoosh
    //https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix


    double inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        m[i] = inv[i] * det;

}

/*void Matrix4::invert(){



    // Create an identity matrix to use as the augmented matrix
    Matrix4 augment;
    for (int i = 0; i < 16; ++i) {
        augment.m[i] = 0.0f;
    }
    for (int i = 0; i < 4; ++i) {
        augment.m[i * 5] = 1.0f;
    }

    // Perform Gauss-Jordan elimination
    for (int col = 0; col < 4; ++col) {
        // Find the pivot row
        int pivotRow = col;
        for (int row = col + 1; row < 4; ++row) {
            if (std::abs(this->m[row * 4 + col]) > std::abs(this->m[pivotRow * 4 + col])) {
                pivotRow = row;
            }
        }

        // Swap rows to get the pivot in place
        swapRows(col, pivotRow);
        swapRows(col, pivotRow);

        // Make the diagonal element 1
        float pivotElement = this->m[col * 4 + col];
        rowReduce(col, pivotElement);
        rowReduce(col, pivotElement);

        // Eliminate other entries in the current column
        for (int row = 0; row < 4; ++row) {
            if (row != col) {
                float factor = this->m[row * 4 + col];
                for (int i = 0; i < 4; ++i) {
                    this->m[row * 4 + i] -= factor * this->m[col * 4 + i];
                    augment.m[row * 4 + i] -= factor * augment.m[col * 4 + i];
                }
            }
        }
    }

    // Extract the inverted matrix from the augmented matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            this->m[i * 4 + j] = augment.m[i * 4 + j + 4];
        }
    }


}
*/