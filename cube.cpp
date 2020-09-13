#include "pch.h"

#include <algorithm>

#include "cube.h"



// we always pre-multiply the defalut x-form matrix with the one being added
// this ensures that post-multiplication with a vector will apply the xforms 
// in the correct order.
void cube::xform(matrix4x4 T)
{
    m_matXform = T * m_matXform;            // start chain with the default matrix
}

// return false if ray doesn't intersect object
// if ray intersects object, smallest positive value is placed first
bool cube::intersect(const ray& r, pInterInfo pint)
{
    bool bRet = false;

    // apply inverse xform to ray so we can pretend our cube is centered at centered at origin...
    ray r1 = const_cast<ray&>(r);
    r1.xform(m_matXform.inverse());


    std::pair<float, float>xcords = checkAxis(const_cast<ray&>(r1).origin().x(), const_cast<ray&>(r1).direction().x());
    std::pair<float, float>ycords = checkAxis(const_cast<ray&>(r1).origin().y(), const_cast<ray&>(r1).direction().y());
    std::pair<float, float>zcords = checkAxis(const_cast<ray&>(r1).origin().z(), const_cast<ray&>(r1).direction().z());

    pint->m_nt1 = std::max({xcords.first, ycords.first, zcords.first}); 
    pint->m_nt2 = std::min({xcords.second, ycords.second, zcords.second });

    if (pint->m_nt1 > pint->m_nt2) bRet = false;
    else bRet = true;

    return bRet;
}

// determines the local normal.  a point on a face will have the largest component of the point equal to +/- 1
vector cube::localNormalAt(point pt)
{
    vector retVector;
    float maxcoor = std::max({ fabs(pt.x()), fabs(pt.y()), fabs(pt.z()) });

    if (maxcoor == fabs(pt.x())) retVector = vector(pt.x(), 0.0f, 0.0f);
    else if (maxcoor == fabs(pt.y())) retVector = vector(0.0f, pt.y(), 0.0f);
    else retVector = vector(0.0f, 0.0f, pt.z());

    return retVector;
}

vector cube::normalAt(point worldPt)
{
    point modelPt = m_matXform.inverse() * worldPt;             //convert from world to model coordinates
    vector modelN = localNormalAt(modelPt);                     // calculate normal in model coordinates
    vector worldN = (m_matXform.inverse()).transpose()* modelN; // convert from model to world coordinates

    worldN.w(0.0f);                                             // fix up homologeous coordinate, 
    worldN.normalize();                                         // normalize the normal

    return worldN;
}


/* checks the two parallel axis's against the ray  The smallest intersection is return in the 
   first element of the pair, and the largest is returned in the second element of the pair
   loc -- [in] the corresponing component of the origin of the ray 
   dir -- [in] the cooresonding component of the direction of the ray
   */
std::pair<float, float> cube::checkAxis(float loc, float dir)
{
    std::pair<float, float>   retPair(0.0f, 0.0f);
    float tminNum = (-1.0f - loc);
    float tmaxNum = (1.0f - loc);

    if (fabs(dir) >= EPSILON)
    {
        retPair.first = tminNum / dir;
        retPair.second = tmaxNum / dir;
    }
    else
    {
        retPair.first = tminNum * INFINITY;
        retPair.second = tmaxNum * INFINITY;
    }

    if (retPair.first > retPair.second) std::swap(retPair.first, retPair.second);

    return retPair;
}

std::ostream& operator<<(std::ostream& os, const cube& s)
{
    os << "id: " << const_cast<cube&>(s).getID() << std::endl;
    os << "   center at: " << const_cast<cube&>(s).getCenter() << std::endl;
    //os << "   with radius: " << const_cast<cube&>(s).getRadius() << std::endl;
    os << *(const_cast<cube&>(s).getMat());
    os << "\ntransform used is\n" << const_cast<cube&>(s).xform();

    return os;

}