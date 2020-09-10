#include "pch.h"
#include "matrix4x4.h"
#include "matrix3x3.h"
#include "matrix2x2.h"

#include "vector.h"
#include "point.h"

#include <iostream>
#include <iomanip>

#define MAKENDX(r,c,w) (r)*(w) + (c)

const int size = 4;
const int size2 = 16;

matrix4x4::matrix4x4() : m_nRows(size), m_nCols(size)//, m_a(nullptr)
{
    //m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
        m_a[ndx] = 0.0f;
}

matrix4x4::matrix4x4(float a, float b, float c, float d,
                     float e, float f, float g, float h,
                     float i, float j, float k, float l,
                     float m, float n, float o, float p) : m_nRows(size), m_nCols(size)//, m_a(nullptr)
{
    //m_a = new float[size2];
    m_a[0] = a;     m_a[1] = b;    m_a[2] = c;   m_a[3] = d;     
    m_a[4] = e;     m_a[5] = f;    m_a[6] = g;   m_a[7] = h;
    m_a[8] = i;     m_a[9] = j;    m_a[10] = k;  m_a[11] = l;
    m_a[12] = m;    m_a[13] = n;   m_a[14] = o;  m_a[15] = p;
}

matrix4x4::matrix4x4(const matrix4x4& rhs)
{
    m_nRows = rhs.m_nRows;
    m_nCols = rhs.m_nCols;

    //m_a = new float[size2];
    for (int ndx = 0; ndx < size2; ndx++)
    {
        m_a[ndx] = rhs.m_a[ndx];
    }
}

matrix4x4 matrix4x4::operator=(const matrix4x4& rhs)
{
    if (*this != rhs)           // check for self-assignment
    {
        m_nRows = rhs.m_nRows;
        m_nCols = rhs.m_nCols;
        //m_a = new float[size2];
        for (int ndx = 0; ndx < size2; ndx++)
        {
            m_a[ndx] = rhs.m_a[ndx];
        }
    }

    return *this;
}
matrix4x4::~matrix4x4()
{
    //if (m_a != nullptr)
    //{
    //    delete[] m_a;
    //    m_a = nullptr;
    //}
}

void  matrix4x4::setElement(int r, int c, float v)
{
    m_a[MAKENDX(r, c, size)] = v;
}

float matrix4x4::getElement(int r, int c)
{
    return m_a[MAKENDX(r, c, size)];
}

void matrix4x4::setElement(int n, float v)
{
    m_a[n]=v;
}

matrix4x4 matrix4x4::operator+(const matrix4x4& rhs)
{
    matrix4x4 sum;

    for (int ndx = 0; ndx < size2; ndx++)
        sum.m_a[ndx] = m_a[ndx] + rhs.m_a[ndx];

    return sum;
}

matrix4x4 matrix4x4::operator-(const matrix4x4& rhs)
{
    matrix4x4 diff;

    for (int ndx = 0; ndx < size2; ndx++)
        diff.m_a[ndx] = m_a[ndx] - rhs.m_a[ndx];

    return diff;
}

// c_ij = \sum_{k=0}^2 a_{ik} * b_{kj}  lhs.col == rhs.row
matrix4x4 matrix4x4::operator*(const matrix4x4& rhs)
{   
    matrix4x4 prod;
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            float sum = 0.0f;
            for (int k = 0; k < size; k++)
            {
                sum += getElement(row, k)*const_cast<matrix4x4&>(rhs).getElement(k, col);
            }
            prod.setElement(row, col,sum);
        }
    }

    return prod;
}

bool matrix4x4::operator==(const matrix4x4& rhs)
{
    bool bRet = true;

    for (int ndx = 0; ndx < size2; ndx++)
        bRet = bRet && (m_a[ndx] == rhs.m_a[ndx]);
    return bRet;
}

bool matrix4x4::operator!=(const matrix4x4& rhs)
{
    return (!(*this == rhs));
}

// given row and col we need to skip those entries
matrix3x3 matrix4x4::minor(int r, int c)
{
    matrix3x3 minor;
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
float matrix4x4::cofactor(int row, int col)
{
    return m_a[MAKENDX(row, col, size)] * ((row + col) % 2 == 0 ? 1.0f : -1.0f);
}

float matrix4x4::det()
{
    float f1 = cofactor(0, 0) * minor(0, 0).det();
    float f2 = cofactor(0, 1) * minor(0, 1).det();
    float f3 = cofactor(0, 2) * minor(0, 2).det();
    float f4 = cofactor(0, 3) * minor(0, 3).det();
    float ret = f1 + f2 + f3+ f4;
    return ret;
}

matrix4x4 matrix4x4::transpose()
{
    matrix4x4 T;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            T.m_a[MAKENDX(col, row, size)] = m_a[MAKENDX(row, col, size)];
        }
    }
    return T;
}

matrix4x4 matrix4x4::inverse()
{
    matrix4x4 I;
    float D = det();

    if (fabsf(D) < EPSILON)
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

matrix4x4 matrix4x4::ident()
{
    matrix4x4  e;
    e.m_a[0] = 1;
    e.m_a[5] = 1;
    e.m_a[10] = 1;
    e.m_a[15] = 1;

    return e;
}

matrix4x4 matrix4x4::identity()
{
    matrix4x4 e;
    e.setElement(0, 1.0f);
    e.setElement(5, 1.0f);
    e.setElement(10, 1.0f);
    e.setElement(15, 1.0f);

    return e;
}


std::ostream& operator<<(std::ostream& os, const matrix4x4& m)
{
    for (int row = 0; row < size; row++)
    {
        os << "| ";
        for (int col = 0; col < size; col++)
        {
            float a = const_cast<matrix4x4&>(m).getElement(MAKENDX(row, col, size));
            if (fabs(a) < EPSILON) a = 0.0f;

            os << std::setw(8) << std::setprecision(5) << a << " ";
        }
        os << " |\n";
    }
    return os;
}

vector operator*(matrix4x4 m, vector v)     // post-multiply by vector
{
    vector   prod;
  
    prod.x(m.a00()*v.x() + m.a01()*v.y() + m.a02()*v.z() + m.a03()*v.w());
    prod.y(m.a10()*v.x() + m.a11()*v.y() + m.a12()*v.z() + m.a13()*v.w());
    prod.z(m.a20()*v.x() + m.a21()*v.y() + m.a22()*v.z() + m.a23()*v.w());
    prod.w(m.a30()*v.x() + m.a31()*v.y() + m.a32()*v.z() + m.a33()*v.w());
    return prod;
}

point operator*(matrix4x4 m, point p)
{
    point prod;
    prod.x(m.a00()*p.x() + m.a01()*p.y() + m.a02()*p.z() + m.a03()*p.w());
    prod.y(m.a10()*p.x() + m.a11()*p.y() + m.a12()*p.z() + m.a13()*p.w());
    prod.z(m.a20()*p.x() + m.a21()*p.y() + m.a22()*p.z() + m.a23()*p.w());
    prod.w(m.a30()*p.x() + m.a31()*p.y() + m.a32()*p.z() + m.a33()*p.w());
    return prod;
}