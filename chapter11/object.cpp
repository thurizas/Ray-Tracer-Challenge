#include "pch.h"

#include "object.h"

int object::m_nCount = 0;                 // total count of object created

object::object(material* pm, unsigned short t) : m_pMat(pm), m_nType(t)
{
    m_xform = matrix4x4::identity();             // set transformation matrix to idenity
    //m_nID = getNextId();
}

object::~object()
{
    if (nullptr != m_pMat)
    {
        delete m_pMat;
    }

}

std::ostream& operator<<(std::ostream& os, const interInfo& info)
{
    if (const_cast<interInfo&>(info).getCnt() > 0)
    {
        os << const_cast<interInfo&>(info).getCnt() << " intersections, at: ";
        os << const_cast<interInfo&>(info).t1();
        if (const_cast<interInfo&>(info).getCnt() > 1)
        {
            os << ", and ";
            os << const_cast<interInfo&>(info).t2();
        }

        os <<" reflected ray is: " << const_cast<interInfo&>(info).reflectv();

    }
    else
    {
        os << "no intersections";
    }
    return os;
}