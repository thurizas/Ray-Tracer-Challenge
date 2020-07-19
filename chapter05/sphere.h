#pragma once

#include "pch.h"

#include "object.h"
#include "ray.h"
#include "point.h"
#include "matrix4x4.h"

#include <iostream>


class sphere : object
{
public:
    sphere(point pt, float r, unsigned short t = SPHERE) : object(t), m_ptCenter(pt), m_fRadius(r), m_matXform(matrix4x4::identity())
    { }

    point getCenter() { return m_ptCenter; }
    float getRadius() { return m_fRadius; }
    int   getID() { return m_nID; }
    matrix4x4 getXform() { return m_matXform; }

    void setXform(matrix4x4);
    bool intersect(const ray&, pInterInfo);

private:
    point     m_ptCenter;
    float     m_fRadius;
    matrix4x4 m_matXform;
};

std::ostream& operator<<(std::ostream&, const sphere&);
