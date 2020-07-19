#include "pch.h"

#include "plane.h"
#include "vector.h"
#include "ray.h"

void plane::xform(matrix4x4 T)
{
    m_matXform = T * m_matXform;            // start chain with the default matrix
}

bool plane::intersect(const ray& r, pInterInfo pInter)
{
    bool bRet = false;
    vector d = const_cast<ray&>(r).direction();
    point  o = const_cast<ray&>(r).origin();

    float denom = m_vecNormal.dot(d);

    if (fabs(denom) > EPSILON)             // intersection happened
    {
        float t = (m_ptCenter - o).dot(m_vecNormal) / denom;
        if (t >= 0)
        {
            pInter->m_nCnt = 1;
            pInter->m_nt1 = t;
            bRet = true;
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

vector plane::normalAt(point worldPt)
{
    point modelPt = m_matXform.inverse() * worldPt;             //convert from world to model coordinates
    vector modelN = localNormalAt(modelPt);                     // calculate normal in model coordinates
    vector worldN = (m_matXform.inverse()).transpose()* modelN; // convert from model to world coordinates

    worldN.w(0.0f);                                             // fix up homologeous coordinate, 
    worldN.normalize();                                         // normalize the normal

    return worldN;
}

vector plane::localNormalAt(point)
{
    return vector(0, 1, 0);        // assuming plane is in xz plane, normal point up(y direction)
}

std::ostream& operator<<(std::ostream& os, const plane& s)
{
    os << "plane: " << const_cast<plane&>(s).getID() << " center at: ";
    os << const_cast<plane&>(s).getCenter();
    os << " with normal: " << const_cast<plane&>(s).getNormal();
    os << "\n" << *(const_cast<plane&>(s).getMat());
    os << "\ntransform used is\n" << const_cast<plane&>(s).xform();

    return os;

}