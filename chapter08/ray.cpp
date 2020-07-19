#include "pch.h"

#include "ray.h"

#include <iostream>


point ray::distance(float t)
{
    point loc;

    loc = m_origin + t * m_direction;
    return loc;
}


void ray::intersect(object* pObject)
{
    interInfo  intInfo;
    if (pObject->intersect(*this, &intInfo))
    {
        m_intersections.push_back(intInfo);
    }
}

void ray::xform(matrix4x4 t)
{
    m_origin = t * m_origin;
    m_direction = t * m_direction;

}



std::ostream& operator<<(std::ostream& os, const ray& r)
{
    os << "origin: " << const_cast<ray&>(r).origin() << 
          " ,direction: " << const_cast<ray&>(r).direction() << std::endl;

    return os;
}