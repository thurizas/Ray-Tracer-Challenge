#include "pch.h"

#include "matrix2x2.h"

#include <iostream>

#define MAKENDX(r,c,w) (r)*(w) + (c)

const int size = 2;
const int size2 = 4;

matrix2x2::matrix2x2() : m_nRows(2), m_nCols(2), m_a(nullptr)
{
    m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
        m_a[ndx] = 0.0f;
}

matrix2x2::matrix2x2(float a, float b, float c, float d) : m_nRows(2), m_nCols(2), m_a(nullptr)
{
    m_a = new float[size2];
    m_a[0] = a;     m_a[1] = b;
    m_a[2] = c;     m_a[3] = d;
}

matrix2x2::matrix2x2(const matrix2x2& rhs)
{
    m_nRows = rhs.m_nRows;
    m_nCols = rhs.m_nCols;

    m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
    {
        m_a[ndx] = rhs.m_a[ndx];
    }
}

matrix2x2 matrix2x2::operator=(const matrix2x2& rhs)
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
matrix2x2::~matrix2x2()
{
    if (m_a != nullptr)
    {
        delete[] m_a;
        m_a = nullptr;
    }
}

void  matrix2x2::setElement(int r, int c, float v)
{
    m_a[MAKENDX(r, c, size)] = v;
}

void matrix2x2::setElement(int n, float v)
{
    m_a[n] = v;
}

matrix2x2 matrix2x2::operator+(const matrix2x2& rhs)
{
    matrix2x2 sum;
    for (int ndx = 0; ndx < size2; ndx++)
        sum.m_a[ndx] = m_a[ndx] + rhs.m_a[ndx];

    return sum;
}

matrix2x2 matrix2x2::operator-(const matrix2x2& rhs)
{
    matrix2x2 diff;
    for (int ndx = 0; ndx < size; ndx++)
        diff.m_a[ndx] = m_a[ndx] + rhs.m_a[ndx];
    return diff;
}

matrix2x2 matrix2x2::operator*(const matrix2x2& rhs)
{
    matrix2x2 prod;
    if ((m_nCols == rhs.m_nRows))
    {
        prod.m_a[0] = m_a[0] * rhs.m_a[0] + m_a[1] * rhs.m_a[2];
        prod.m_a[1] = m_a[0] * rhs.m_a[1] + m_a[1] * rhs.m_a[3];
        prod.m_a[2] = m_a[2] * rhs.m_a[0] + m_a[3] * rhs.m_a[2];
        prod.m_a[3] = m_a[2] * rhs.m_a[1] + m_a[3] * rhs.m_a[3];
    }
    else
    {
        std::cerr << "invalid dimensions, columns of first != rows of second" << std::endl;
    }
    return prod;
}

bool matrix2x2::operator==(const matrix2x2& rhs)
{
    return((m_a[0] == rhs.m_a[0]) && (m_a[1] == rhs.m_a[1]) &&
           (m_a[2] == rhs.m_a[2]) && (m_a[3] == rhs.m_a[3]));
}

bool matrix2x2::operator!=(const matrix2x2& rhs)
{
    return (!(*this == rhs));
}

float     matrix2x2::det()
{
    float ret = m_a[0] * m_a[3] - m_a[1] * m_a[2];
    return ret;
}

matrix2x2 matrix2x2::transpose()
{
    matrix2x2 T;
    T.m_a[0] = m_a[0];
    T.m_a[1] = m_a[2];
    T.m_a[2] = m_a[1];
    T.m_a[3] = m_a[3];
    return T;
}

matrix2x2 matrix2x2::inverse()
{
    float D = det();
    matrix2x2 I;
    if (fabs(D) < EPSILON)
    {
        std::cerr << "matrix is singular" << std::endl;
    }
    else
    {
        float f = 1.0f / D;
        I.m_a[0] = f* m_a[3];
        I.m_a[1] = - f*m_a[1];
        I.m_a[2] = -f* m_a[2];
        I.m_a[3] = f* m_a[0];
    }
    return I;
}

matrix2x2 matrix2x2::ident()
{
    matrix2x2  e;
    e.m_a[0] = 1;
    e.m_a[3] = 1;

    return e;
}


std::ostream& operator<<(std::ostream& os, const matrix2x2& m)
{
    os << "| " << const_cast<matrix2x2&>(m).a00() << " " << const_cast<matrix2x2&>(m).a01() << " |\n";
    os << "| " << const_cast<matrix2x2&>(m).a10() << " " << const_cast<matrix2x2&>(m).a11() << " |\n";

    return os;
}