#pragma once
#include "pch.h"

#include "object.h"
#include "ray.h"
#include "vector.h"
#include "point.h"
#include "matrix4x4.h"


class cube : public object
{
public:
    cube(material* pm = new material, unsigned short t = CUBE) : object(pm, t), m_ptCenter(0, 0, 0), m_vecNormal(0, 1, 0), m_matXform(matrix4x4::identity())
    { }

    ~cube() { if (m_pMat != nullptr) { delete m_pMat; m_pMat = nullptr; } }

    point getCenter() { return m_ptCenter; }
    vector getNormal() { return m_vecNormal; }

    matrix4x4 xform() { return m_matXform; }
    void      xform(matrix4x4);

    material* getMat() { return m_pMat; }
    void      mat(material* pm) { m_pMat = pm; }


    int       getID() { return m_nID; }
    // TODO : needs to be -- pCube->intersect(r, &intInfo)  intInfo is 
    bool intersect(const ray&, pInterInfo);
    vector normalAt(point worldPt);
    vector localNormalAt(point);

private:
    point     m_ptCenter;
    vector    m_vecNormal;
    matrix4x4 m_matXform;

    std::pair<float, float> checkAxis(float, float);

};

std::ostream& operator<<(std::ostream&, const cube&);

