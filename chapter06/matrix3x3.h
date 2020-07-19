#pragma once

#include <iostream>

class matrix2x2;

class matrix3x3
{
public:
    matrix3x3();
    matrix3x3(float, float, float, float, float, float, float, float, float);
    matrix3x3(const matrix3x3&);
    matrix3x3 operator=(const matrix3x3&);
    ~matrix3x3();

    float a00() { return m_a[0]; }
    void  a00(float v) { m_a[0] = v; }
    float a01() { return m_a[1]; }
    void  a01(float v) { m_a[1] = v; }
    float a02() { return m_a[2]; }
    void  a02(float v) { m_a[2] = v; }
    float a10() { return m_a[3]; }
    void  a10(float v) { m_a[3] = v; }
    float a11() { return m_a[4]; }
    void a11(float v) { m_a[4] = v; }
    float a12() { return m_a[5]; }
    void a12(float v) { m_a[5] = v; }
    float a20() { return m_a[6]; }
    void  a20(float v) { m_a[6] = v; }
    float a21() { return m_a[7]; }
    void a21(float v) { m_a[7] = v; }
    float a22() { return m_a[8]; }
    void a22(float v) { m_a[8] = v; }

    void setElement(int r, int c, float v);
    void setElement(int, float v);

    matrix3x3 operator+(const matrix3x3&);
    matrix3x3 operator-(const matrix3x3&);
    matrix3x3 operator*(const matrix3x3&);

    bool      operator==(const matrix3x3&);
    bool      operator!=(const matrix3x3&);

    float     det();
    matrix3x3 transpose();
    matrix3x3 inverse();
    matrix3x3 ident();
    matrix2x2 minor(int, int);
    float cofactor(int, int);

private:
    int   m_nRows;
    int   m_nCols;
    float* m_a;
};

std::ostream& operator<<(std::ostream&, const matrix3x3&);
