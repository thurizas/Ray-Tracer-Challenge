#pragma once


#include "color.h"
#include "point.h"
#include "material.h"
#include "vector.h"

class light
{
public:
    light() : m_intensity(color(1.0f, 1.0f, 1.0f)), m_ptLoc(point(0.0f, 0.0f, 0.0f)) { }
    light(color c, point p) : m_intensity(c), m_ptLoc(p) { }

    color i() { return m_intensity; }
    void  i(color c) { m_intensity = c; }
    point p() { return m_ptLoc; }
    void  p(point pt) { m_ptLoc = pt; }

    color lighting(material m, point pt, vector eyev, vector normalv);

private:
    color m_intensity;
    point m_ptLoc;
};

std::ostream operator<<(std::ostream, const light&);
