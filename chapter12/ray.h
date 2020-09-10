#pragma once

#include "pch.h"

#include "point.h"
#include "vector.h"
#include "matrix4x4.h"
#include "object.h"


#include <iostream>

class ray
{
public:
    ray() : m_origin(0, 0, 0), m_direction(0, 0, 0) { }
    ray(point pt, vector v) : m_origin(pt), m_direction(v)
    { }

    point origin() { return m_origin; }
    void origin(const point pt) { m_origin = pt; }
    vector direction() { return m_direction; }
    void direction(const vector v) { m_direction = v; }
    void xform(matrix4x4);

    point distance(float t);
    interInfo  intersect(object*);


private:
    point                  m_origin;
    vector                 m_direction;
};

std::ostream& operator<<(std::ostream&, const ray&);