#pragma once

#include "pch.h"

#include "object.h"
#include "ray.h"
#include "vector.h"
#include "point.h"
#include "matrix4x4.h"

#include <iostream>


class sphere : public object
{
public:
    sphere(point pt, float r = 1.0f, material* pm = new material, unsigned short t = SPHERE) : object(pm, t), m_ptCenter(pt), m_fRadius(r), m_matXform(m_xform)
    { }

    point     getCenter() { return m_ptCenter; }
    float     getRadius() { return m_fRadius; }

    matrix4x4 getXform() { return m_matXform; }
    void      xform(matrix4x4);

    material* getMat() { return m_pMat; }
    void      mat(material* pm) { m_pMat = pm; }


    int       getID() { return m_nID; }

    bool intersect(const ray&, pInterInfo);
    vector normalAt(point worldPt);

private:
    point     m_ptCenter;
    float     m_fRadius;
    matrix4x4 m_matXform;

};

std::ostream& operator<<(std::ostream&, const sphere&);
