#pragma once

#undef minor
#undef major

#include <iostream>

#include "matrix3x3.h"
#include "matrix2x2.h"

class matrix2x2;
class matrix3x3;

class vector;
class point;

class matrix4x4
{
public:
    matrix4x4();
    matrix4x4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    matrix4x4(const matrix4x4&);
    matrix4x4 operator=(const matrix4x4&);
    ~matrix4x4();

    float a00() { return m_a[0]; }
    void  a00(float v) { m_a[0] = v; }
    float a01() { return m_a[1]; }
    void  a01(float v) { m_a[1] = v; }
    float a02() { return m_a[2]; }
    void  a02(float v) { m_a[2] = v; }
    float a03() { return m_a[3]; }
    void  a03(float v) { m_a[3] = v; }
    float a10() { return m_a[4]; }
    void  a10(float v) { m_a[4] = v; }
    float a11() { return m_a[5]; }
    void  a11(float v) { m_a[5] = v; }
    float a12() { return m_a[6]; }
    void  a12(float v) { m_a[6] = v; }
    float a13() { return m_a[7]; }
    void  a13(float v) { m_a[7] = v; }
    float a20() { return m_a[8]; }
    void  a20(float v) { m_a[8] = v; }
    float a21() { return m_a[9]; }
    void  a21(float v) { m_a[9] = v; }
    float a22() { return m_a[10]; }
    void  a22(float v) { m_a[10] = v; }
    float a23() { return m_a[11]; }
    void  a23(float v) { m_a[11] = v; }
    float a30() { return m_a[12]; }
    void  a30(float v) { m_a[12] = v; }
    float a31() { return m_a[13]; }
    void  a31(float v) { m_a[13] = v; }
    float a32() { return m_a[14]; }
    void  a32(float v) { m_a[14] = v; }
    float a33() { return m_a[15]; }
    void  a33(float v) { m_a[15] = v; }

    void setElement(int r, int c, float v); 
    float getElement(int, int);
    void setElement(int, float v);
    float getElement(int);

    matrix4x4 operator+(const matrix4x4&);
    matrix4x4 operator-(const matrix4x4&);
    matrix4x4 operator*(const matrix4x4&);

    bool      operator==(const matrix4x4&);
    bool      operator!=(const matrix4x4&);

    float     det();
    matrix4x4 transpose();
    matrix4x4 inverse();
    matrix4x4 ident();
    static matrix4x4 identity();    // creates an idenity matrix
    matrix3x3 minor(int, int);
    float cofactor(int, int);


private:
    int   m_nRows;
    int   m_nCols;
    float* m_a;
};

std::ostream& operator<<(std::ostream&, const matrix4x4&);
vector operator*(matrix4x4, vector);     // post-multiply by vector
point operator*(matrix4x4, point);       // post-multiply by point
