#include "pch.h"

#include "sphere.h"
#include "ray.h"

// return false if ray doesn't intersect object
// if ray intersects object, smallest positive value is placed first
bool sphere::intersect(const ray& r, pInterInfo pint)
{
    // apply inverse xform to ray so we can pretend out sphere is a unit sphere at origin...
    ray r1 = const_cast<ray&>(r); 
    r1.xform(m_matXform.inverse());
    

    bool bRet = false;
    pint->m_nID = m_nID;

    vector v1 = r1.origin() - point(0,0,0);  // vector from ray origin to sphere center

    float a = r1.direction().dot(r1.direction());
    float b = 2 * r1.direction().dot(v1);
    float c = v1.dot(v1) - 1;                  // TODO : should 1 be r^2?

    float d = b * b - 4 * a*c;

    if (d < 0)
    {
        pint->m_nCnt = 0;
        pint->m_nt1 = pint->m_nt2 = 0.0f;
    }
    else
    {
        float pt1, pt2;
        pint->m_nCnt = 2;
        pt1 = (-b - sqrt(d)) / (2 * a);
        pt2 = (-b + sqrt(d)) / (2 * a);

        if (fabsf(pt1 - pt2) < EPSILON)            // points are equal
        {
            pint->m_nt1 = pint->m_nt2 = pt1;
        }
        else if ((pt1 > 0) && (pt2 > 0))            // both intersections are in-front of origin
        {
            (pt1 > pt2 ? (pint->m_nt1 = pt2, pint->m_nt2 = pt1) : (pint->m_nt1 = pt1, pint->m_nt2 = pt2));
        }
        else if ((pt1 < 0) && (pt2 < 0))            // both intersections are behind of origin
        {
            (pt1 > pt2 ? (pint->m_nt1 = pt2, pint->m_nt2 = pt1) : (pint->m_nt1 = pt1, pint->m_nt2 = pt2));
        }
        else                                        // origin is in object
        {
            (pt2 > 0 ? (pint->m_nt1 = pt2, pint->m_nt2 = pt1) : (pint->m_nt1 = pt1, pint->m_nt2 = pt2));
        }
        bRet =  true;
    }

    return bRet;
}

// we always pre-multiply the defalut x-form matrix with the one being added
// this ensures that post-multiplication with a vector will apply the xforms 
// in the correct order.
void sphere::xform(matrix4x4 T)
{
   m_matXform = T * m_matXform;            // start chain with the default matrix
}


vector sphere::normalAt(point worldPt)
{
    point modelPt = m_matXform.inverse() * worldPt;             //convert from world to model coordinates
    vector modelN = modelPt - point(0, 0, 0);                   // calculate normal in model coordinates
    vector worldN = (m_matXform.inverse()).transpose()* modelN; // convert from model to world coordinates

    worldN.w(0.0f);                                             // fix up homologeous coordinate, 
    worldN.normalize();                                         // normalize the normal

    return worldN;
}





std::ostream& operator<<(std::ostream& os, const sphere& s)
{
    os << "sphere: " << const_cast<sphere&>(s).getID() << " center at: ";
    os << const_cast<sphere&>(s).getCenter();
    os << " with radius: " << const_cast<sphere&>(s).getRadius();
    os << "\n" << *(const_cast<sphere&>(s).getMat());
    os << "\ntransform used is\n" << const_cast<sphere&>(s).getXform();

    return os;

}