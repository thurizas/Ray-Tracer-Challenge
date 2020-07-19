#include "pch.h"
#include "matrix3x3.h"
#include "matrix2x2.h"

#include <iostream>

#define MAKENDX(r,c,w) (r)*(w) + (c)

const int size = 3;
const int size2 = 9;

matrix3x3::matrix3x3() : m_nRows(3), m_nCols(3), m_a(nullptr)
{
    m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
        m_a[ndx] = 0.0f;
}

matrix3x3::matrix3x3(float a, float b, float c, 
                     float d, float e, float f,
                     float g, float h, float i) : m_nRows(3), m_nCols(3), m_a(nullptr)
{
    m_a = new float[size2];
    m_a[0] = a;     m_a[1] = b;    m_a[2] = c;
    m_a[3] = d;     m_a[4] = e;    m_a[5] = f;
    m_a[6] = g;     m_a[7] = h;    m_a[8] = i;
}

matrix3x3::matrix3x3(const matrix3x3& rhs)
{
    m_nRows = rhs.m_nRows;
    m_nCols = rhs.m_nCols;

    m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
    {
        m_a[ndx] = rhs.m_a[ndx];
    }
}

matrix3x3 matrix3x3::operator=(const matrix3x3& rhs)
{
    if (*this != rhs)           // check for self-assignment
    {
        m_nRows = rhs.m_nRows;
        m_nCols = rhs.m_nCols;
        m_a = new float[size2];
        for (int ndx = 0; ndx < size2; ndx++)
        {
            m_a[ndx] = rhs.m_a[ndx];
        }
    }

    return *this;
}
matrix3x3::~matrix3x3()
{
    if (m_a != nullptr)
    {
        delete[] m_a;
        m_a = nullptr;
    }
}

void  matrix3x3::setElement(int r, int c, float v)
{
    m_a[MAKENDX(r, c, size)] = v;
}

void matrix3x3::setElement(int n, float v)
{
    m_a[n] = v;
}

matrix3x3 matrix3x3::operator+(const matrix3x3& rhs)
{
    matrix3x3 sum;

    for (int ndx = 0; ndx < size2; ndx++)
        sum.m_a[ndx] = m_a[ndx] + rhs.m_a[ndx];

    return sum;
}

matrix3x3 matrix3x3::operator-(const matrix3x3& rhs)
{
    matrix3x3 diff;

    for (int ndx = 0; ndx < size2; ndx++)
        diff.m_a[ndx] = m_a[ndx] - rhs.m_a[ndx];

    return diff;
}

// c_ij = \sum_{k=0}^2 a_{ik} * b_{kj}
matrix3x3 matrix3x3::operator*(const matrix3x3& rhs)
{
    matrix3x3 prod;
    prod.m_a[0] = m_a[0] * rhs.m_a[0] + m_a[1] * rhs.m_a[3] + m_a[2] * rhs.m_a[6];
    prod.m_a[1] = m_a[0] * rhs.m_a[1] + m_a[1] * rhs.m_a[4] + m_a[2] * rhs.m_a[7];
    prod.m_a[2] = m_a[0] * rhs.m_a[2] + m_a[1] * rhs.m_a[5] + m_a[2] * rhs.m_a[8];

    prod.m_a[3] = m_a[3] * rhs.m_a[0] + m_a[4] * rhs.m_a[3] + m_a[5] * rhs.m_a[6];
    prod.m_a[4] = m_a[3] * rhs.m_a[1] + m_a[4] * rhs.m_a[4] + m_a[5] * rhs.m_a[7];
    prod.m_a[5] = m_a[3] * rhs.m_a[2] + m_a[4] * rhs.m_a[5] + m_a[5] * rhs.m_a[8];

    prod.m_a[6] = m_a[6] * rhs.m_a[0] + m_a[7] * rhs.m_a[3] + m_a[8] * rhs.m_a[6];
    prod.m_a[7] = m_a[6] * rhs.m_a[1] + m_a[7] * rhs.m_a[4] + m_a[8] * rhs.m_a[7];
    prod.m_a[8] = m_a[6] * rhs.m_a[2] + m_a[7] * rhs.m_a[5] + m_a[8] * rhs.m_a[8];

    return prod;
}

bool matrix3x3::operator==(const matrix3x3& rhs)
{
    bool bRet = true;

    for (int ndx = 0; ndx < size2; ndx++)
        bRet = bRet && (m_a[ndx] == rhs.m_a[ndx]);
    return bRet;
}

bool matrix3x3::operator!=(const matrix3x3& rhs)
{
    return (!(*this == rhs));
}

// given row and col we need to skip those entries
matrix2x2 matrix3x3::minor(int r, int c)
{
    matrix2x2 minor;
    int ndx = 0;
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        { 
            if (row == r || col == c)
                continue;
            else
            {
                minor.setElement(ndx++, m_a[MAKENDX(row, col, size)]);
            }
        }
    }
    return minor;
}

// (-1)^{i + j}
float matrix3x3::cofactor(int row, int col)
{
    return m_a[MAKENDX(row, col, size)]*((row + col) % 2 == 0 ? 1.0f : -1.0f);
}

float matrix3x3::det()
{
    float f1 = cofactor(0, 0) * minor(0, 0).det();
    float f2 = cofactor(0, 1) * minor(0, 1).det();
    float f3 = cofactor(0, 2) * minor(0, 2).det();
    float ret = f1 + f2 + f3;
    return ret;
}

matrix3x3 matrix3x3::transpose()
{
    matrix3x3 T;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            T.m_a[MAKENDX(col, row, size)] = m_a[MAKENDX(row, col, size)];
        }
    }
    return T;
}
 
matrix3x3 matrix3x3::inverse()
{
    matrix3x3 I;
    float D = det();
    
    if (fabs(D) < EPSILON)
    {
        std::cerr << "matrix is singular" << std::endl;
    }
    else
    {
        float f = 1.0f / D;
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                float sgn = ((row + col) % 2 == 0 ? 1.0f : -1.0f);
                float c = sgn * minor(row, col).det();
                 I.m_a[MAKENDX(col, row, size)] = c * f;
            }
        }
    }
    return I;
}

matrix3x3 matrix3x3::ident()
{
    matrix3x3  e;
    e.m_a[0] = 1;
    e.m_a[4] = 1;
    e.m_a[8] = 1;

    return e;
}


std::ostream& operator<<(std::ostream& os, const matrix3x3& m)
{
    os << "| " << const_cast<matrix3x3&>(m).a00() << " " << const_cast<matrix3x3&>(m).a01() << " " << const_cast<matrix3x3&>(m).a02() << " |\n";
    os << "| " << const_cast<matrix3x3&>(m).a10() << " " << const_cast<matrix3x3&>(m).a11() << " " << const_cast<matrix3x3&>(m).a12() << " |\n";
    os << "| " << const_cast<matrix3x3&>(m).a20() << " " << const_cast<matrix3x3&>(m).a21() << " " << const_cast<matrix3x3&>(m).a22() << " |\n";
    return os;
}






