#include "pch.h"

#include "object.h"

int object::m_nCount = 0;                 // total count of object created

object::object(unsigned short t) : m_nType(t)
{
    m_nID = getNextId();
}

std::ostream& operator<<(std::ostream& os, const interInfo& info)
{
    if (const_cast<interInfo&>(info).getCnt() > 0)
    {
        os << const_cast<interInfo&>(info).getCnt() << " intersections, at: ";
        os << const_cast<interInfo&>(info).t1() << ", and ";
        os << const_cast<interInfo&>(info).t2();
    }
    else
    {
        os << "no intersections";
    }
    return os;
}