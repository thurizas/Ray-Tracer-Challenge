
#include "pch.h"

#include "vector.h"
#include "point.h"
#include <cmath>

point::point() : m_x(0), m_y(0), m_z(0), m_w(1)
{  }

point::point(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w)
{  }


point::point(const point& rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_z(rhs.m_z),m_w(rhs.m_w)
{  }

point& point::operator=(const point& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    m_z = rhs.m_z;
    m_w = rhs.m_w;
    return *this;
}


bool point::operator==(const point& rhs)
{
    return ((fabs(m_x - rhs.m_x) < EPSILON) && 
            (fabs(m_y - rhs.m_y) < EPSILON) && 
            (fabs(m_z - rhs.m_z) < EPSILON));

}


// negation operator
point point::operator-()
{
    point pt = point(-m_x, -m_y, -m_z, 1);
    return pt;
}



// point added to vector gives another point 
point point::operator+(const vector& rhs)
{
    float x = const_cast<vector&>(rhs).x();
    float y = const_cast<vector&>(rhs).y();
    float z = const_cast<vector&>(rhs).z();

    point pt = point(m_x + x, m_y + y, m_z + z,1);
    return pt;
}


// subtraction vector frpm point gives another point 
point point::operator-(const vector& rhs)
{
    float x = const_cast<vector&>(rhs).x();
    float y = const_cast<vector&>(rhs).y();
    float z = const_cast<vector&>(rhs).z();

    point pt = point(m_x - x, m_y - y, m_z - z, 1);
    return pt;
}


// subtracting two points give a vector
vector point::operator-(const point& rhs)
{
    vector vec(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z,0);
    return vec;
}

std::ostream& operator<<(std::ostream& os, const point& v)
{
    os << "( " << const_cast<point&>(v).x() << " ,";
    os << const_cast<point&>(v).y() << " ,";
    os << const_cast<point&>(v).z() << " ,";
    os << const_cast<point&>(v).w() << ")";
    return os;
}