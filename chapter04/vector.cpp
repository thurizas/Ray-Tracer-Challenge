#include "pch.h"

#include "vector.h"
#include "point.h"
#include <cmath>


vector::vector() : m_x(0), m_y(0), m_z(0), m_w(1)
{  }

vector::vector(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w)
{  }


vector::vector(const vector& rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_z(rhs.m_z),m_w(rhs.m_w)
{  }

vector& vector::operator=(const vector& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    m_z = rhs.m_z;
    m_w = rhs.m_w;
    return *this;
}

bool vector::operator==(const vector& rhs)
{
    return ((fabs(m_x - rhs.m_x) < EPSILON) &&
            (fabs(m_y - rhs.m_y) < EPSILON) &&
            (fabs(m_z - rhs.m_z) < EPSILON));

}


vector vector::operator-()
{
    vector vec = vector(-m_x, -m_y, -m_z);
    return vec;

}

// adding two vectors gives a vector
vector vector::operator+(const vector& rhs)
{
    vector vec(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
    return vec;
}


// adding a point and vector gives a new point
point vector::operator+(const point& rhs)
{
    const float x = const_cast<point&>(rhs).x();
    const float y = const_cast<point&>(rhs).y();
    const float z = const_cast<point&>(rhs).z();

    point pt(m_x + x, m_y + y, m_z + z);
    return pt;
}


// subtracting two vectors gives a vector
vector vector::operator-(const vector& rhs)
{
    vector vec = vector(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
    return vec;
}


// scaleing by a floating point 
vector vector::operator*(const float f)
{
    return vector(f*m_x, f*m_y, f*m_z, f*m_w);
}


float vector::norm2()
{
    return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w; // should be 3^2 + 2^2 + 1^2 + 0^2 = 9 + 4 + 1 +0 = 14
}


float vector::norm()
{
    return sqrt(m_x*m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}



void vector::normalize()
{
    float scale = this->norm();
    m_x = m_x / scale;
    m_y = m_y / scale;
    m_z = m_z / scale;
    m_w = m_w / scale;
}


float vector::dot(const vector& op2)
{
    return (this->m_x*op2.m_x + this->m_y*op2.m_y + this->m_z*op2.m_z + this->m_w*op2.m_w);

}


vector vector::cross(const vector& op2)
{
    return vector(m_y * op2.m_z - m_z * op2.m_y,
                  m_z * op2.m_x - m_x * op2.m_z,
                  m_x * op2.m_y - m_y * op2.m_x);
}




// non-member funtion to allow float*vector
vector operator*(float f, const vector& vec)
{
    float x = const_cast<vector&>(vec).x();
    float y = const_cast<vector&>(vec).y();
    float z = const_cast<vector&>(vec).z();

    vector v = vector(f*x, f*y, f*z, 0);
    return v;

}

std::ostream& operator<<(std::ostream& os, const vector& v)
{
    os << "| " << const_cast<vector&>(v).x() << " |" << std::endl;
    os << "| " << const_cast<vector&>(v).y() << " |" << std::endl;
    os << "| " << const_cast<vector&>(v).z() << " |" << std::endl;
    os << "| " << const_cast<vector&>(v).w() << " |" << std::endl;
    return os;
}
