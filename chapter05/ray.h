#pragma once

#include "pch.h"

#include "point.h"
#include "vector.h"
#include "matrix4x4.h"
#include "object.h"


#include <iostream>
#include <vector>

class ray
{
public:
    ray(point pt, vector v) : m_origin(pt), m_direction(v)
    { }

    point origin() { return m_origin; }
    vector direction() { return m_direction; }
    void xform(matrix4x4);

    point distance(float t);
    void intersect(object*);


private:
    point   m_origin;
    vector  m_direction;
    std::vector<interInfo> m_intersections;


};

std::ostream& operator<<(std::ostream&, const ray&);
