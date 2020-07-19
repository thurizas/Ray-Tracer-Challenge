#pragma once

#include "pch.h"

class color
{
public:
    color();
    color(float, float, float);
    color(const color&);
    color& operator=(const color& rhs);            // assignment operator

    float r() { return m_r; }
    void  setr(float c) { m_r = c; }
    float g() { return m_g; }
    void setg(float c) { m_g = c; }
    float b() { return m_b; }
    void setb(float b) { m_b = b; }

    color operator+(const color& rhs);
    color operator-(const color& rhs);
    color operator*(const float);
    color operator*(const color& rhs);            // Hadamard product

private:
    float   m_r;
    float   m_g;
    float   m_b;
};

// to allow f*vec
color operator*(float f, const color& c);
color operator*(int, const color& c);      // used in ppm calculations, clamp color  value in [0,255]
std::ostream& operator<<(std::ostream& ox, const color& c);
