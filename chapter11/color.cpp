#include "pch.h"

#include "color.h"

color::color() : m_r(0), m_g(0), m_b(0)
{  }


color::color(float r, float g , float b) : m_r(r), m_g(g), m_b(b)
{  }


color::color(const color& rhs) : m_r(const_cast<color&>(rhs).r()), m_g(const_cast<color&>(rhs).g()), m_b(const_cast<color&>(rhs).b())
{  }

color& color::operator=(const color& rhs)            // assignment operator
{
    m_r = const_cast<color&>(rhs).r();
    m_g = const_cast<color&>(rhs).g();
    m_b = const_cast<color&>(rhs).b();

    return *this;
}



color color::operator+(const color& rhs)
{
    float comp[3];
    comp[0] = m_r + const_cast<color&>(rhs).r();
    comp[1] = m_g + const_cast<color&>(rhs).g();
    comp[2] = m_b + const_cast<color&>(rhs).b();

    for (int ndx = 0; ndx < 3; ndx++)   // keep color components clamped in [0,1]
    {
        if (comp[ndx] > 1) comp[ndx] = 1;
    }

    return color(comp[0], comp[1], comp[2]);
}


color color::operator-(const color& rhs)
{
    float comp[3];
    comp[0] = m_r + const_cast<color&>(rhs).r();
    comp[1] = m_g + const_cast<color&>(rhs).g();
    comp[2] = m_b + const_cast<color&>(rhs).b();

    for (int ndx = 0; ndx < 3; ndx++)   // keep color components clamped in [0,1]
    {
        if (fabs(comp[ndx]) < EPSILON) comp[ndx] = 0.0f;
    }
    return color(comp[0], comp[1], comp[2]);
}

color color::operator*(const float f)
{
    float comp[3];
    comp[0] = f*m_r;
    comp[1] = f * m_g;
    comp[2] = f * m_b;

    for (int ndx = 0; ndx < 3; ndx++)   // keep color components clamped in [0,1]
    {
        if (fabs(comp[ndx]) > 1) comp[ndx] = 1.0f;
    }
    return color(comp[0], comp[1], comp[2]);
}



color color::operator*(const color& rhs)            // Hadamard product
{
    float comp[3];
    comp[0] = m_r * const_cast<color&>(rhs).r();
    comp[1] = m_g * const_cast<color&>(rhs).g();
    comp[2] = m_b * const_cast<color&>(rhs).b();

    for (int ndx = 0; ndx < 3; ndx++)   // keep color components clamped in [0,1]
    {
        if (fabs(comp[ndx]) > 1) comp[ndx] = 1.0f;
    }
    return color(comp[0], comp[1], comp[2]);
}


// to allow f*vec
color operator*(float f, const color& c)
{
    float comp[3];
    comp[0] = f * const_cast<color&>(c).r();
    comp[1] = f * const_cast<color&>(c).g();
    comp[2] = f * const_cast<color&>(c).b();

    for (int ndx = 0; ndx < 3; ndx++)   // keep color components clamped in [0,1]
    {
        if (fabs(comp[ndx]) > 1) comp[ndx] = 1.0f;
    }
    return color(comp[0], comp[1], comp[2]);
}

// used in writting ppm file, clamps color in range [0, 255]
color operator*(int v, const color& c)
{
    float comp[3];                  // array for r-g-b values;

    comp[0] = 255 * (const_cast<color&>(c).r());
    comp[1] = 255 * (const_cast<color&>(c).g());
    comp[2] = 255 * (const_cast<color&>(c).b());

    // clamp components in the rangs [0,255]
    for (int ndx = 0; ndx < 3; ndx++)
    {
        if (comp[ndx] > 255) comp[ndx] = 255;
        if (comp[ndx] < 0) comp[ndx] = 0;
    }

    return color(comp[0], comp[1], comp[2]);
}
/*
    os << "( " << (fabs(const_cast<point&>(v).x()) < EPSILON ? 0 : const_cast<point&>(v).x()) << ", ";
    os << (fabs(const_cast<point&>(v).y()) < EPSILON ? 0 : const_cast<point&>(v).y()) << ", ";
    os << (fabs(const_cast<point&>(v).z()) < EPSILON ? 0 : const_cast<point&>(v).z()) << ", ";
    os << (fabs(const_cast<point&>(v).w()) < EPSILON ? 0 : const_cast<point&>(v).w()) << ")";
    return os;
*/
std::ostream &operator<<(std::ostream &os, const color& c) 
{
    os << "( " << (fabs(const_cast<color&>(c).r()) < EPSILON ? 0 : const_cast<color&>(c).r()) << ", ";
    os << (fabs(const_cast<color&>(c).g()) < EPSILON ? 0 : const_cast<color&>(c).g()) << ", ";
    os << (fabs(const_cast<color&>(c).b()) < EPSILON ? 0 : const_cast<color&>(c).b()) << ")";
    return os;
}