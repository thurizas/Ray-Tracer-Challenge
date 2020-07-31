#include "pch.h"

#include "ray.h"

#include <iostream>


point ray::distance(float t)
{
    point loc;

    loc = m_origin + t * m_direction;
    return loc;
}

/*
 * this function determines if a ray intersects the given object.  This is a stand alone function
 * used for testing.
 */
/*
* function: ray::intersect
*
* abstract: This function determines if a ray intersects a given object.  This is a stand alone function
*           used for testing only.
*
* inputs  : pObject -- [in] a pointer to an object for testings.
*
* returns : return an intersection info structure (see object.h for the definition)
*
* written : (1/25/2020)
* modified: (29Jul2020) Added support for reflection about normal
*/
interInfo ray::intersect(object* pObject)
{
    interInfo  intInfo;
    if (pObject->intersect(*this, &intInfo))
    {
        point  pt = m_origin + intInfo.m_nt1*m_direction;
        intInfo.m_reflectv = m_direction.reflect(pObject->normalAt(pt));
    }

    return intInfo;
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