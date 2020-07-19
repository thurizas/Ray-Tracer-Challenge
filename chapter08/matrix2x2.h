#pragma once

#include <iostream>

class matrix2x2
{
public:
    matrix2x2();
    matrix2x2(float, float, float, float);
    matrix2x2(const matrix2x2&);
    matrix2x2 operator=(const matrix2x2&);
    ~matrix2x2();

    float a00() { return m_a[0]; }
    void  a00(float v) { m_a[0] = v; }
    float a01() { return m_a[1]; }
    void  a01(float v) { m_a[1] = v; }
    float a10() { return m_a[2]; }
    void  a10(float v) { m_a[2] = v; }
    float a11() { return m_a[3]; }
    void a11(float v) { m_a[3] = v; }

    void setElement(int r, int c, float v);
    void setElement(int, float v);

    matrix2x2 operator+(const matrix2x2&);
    matrix2x2 operator-(const matrix2x2&);
    matrix2x2 operator*(const matrix2x2&);

    bool      operator==(const matrix2x2&);
    bool      operator!=(const matrix2x2&);

    float     det();
    matrix2x2 transpose();
    matrix2x2 inverse();
    matrix2x2 ident();

private:
    int   m_nRows;
    int   m_nCols;
    float* m_a;
};

std::ostream& operator<<(std::ostream&, const matrix2x2&);
