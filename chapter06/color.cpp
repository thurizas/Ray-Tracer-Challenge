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
    return color(m_r + const_cast<color&>(rhs).r(), m_g + const_cast<color&>(rhs).g(), m_b + const_cast<color&>(rhs).b());
}


color color::operator-(const color& rhs)
{
    return color(m_r - const_cast<color&>(rhs).r(), m_g - const_cast<color&>(rhs).g(), m_b - const_cast<color&>(rhs).b());
}

color color::operator*(const float f)
{
    return color(m_r * f, m_g * f, m_b * f);
}



color color::operator*(const color& rhs)            // Hadamard product
{
    return color(m_r * const_cast<color&>(rhs).r(), m_g * const_cast<color&>(rhs).g(), m_b * const_cast<color&>(rhs).b());
}


// to allow f*vec
color operator*(float f, const color& c)
{
    return color(f*const_cast<color&>(c).r(), f*const_cast<color&>(c).g(), f*const_cast<color&>(c).b());
}

// used in writting ppm file, clamps color in range [0, 255]
color operator*(int v, const color& c)
{
    int comp[3];                  // array for r-g-b values;

    comp[0] = 255 * (int)(const_cast<color&>(c).r());
    comp[1] = 255 * (int)(const_cast<color&>(c).g());
    comp[2] = 255 * (int)(const_cast<color&>(c).b());

    // clamp components in the rangs [0,255]
    for (int ndx = 0; ndx < 3; ndx++)
    {
        if (comp[ndx] > 255) comp[ndx] = 255;
        if (comp[ndx] < 0) comp[ndx] = 0;
    }

    return color(comp[0], comp[1], comp[2]);
}

std::ostream &operator<<(std::ostream &os, const color& c) 
{
    os << "( " << const_cast<color&>(c).r() << ", ";
    os << const_cast<color&>(c).g() << ", ";
    os << const_cast<color&>(c).b() << ")";
    return os;
}