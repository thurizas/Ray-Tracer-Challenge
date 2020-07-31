#include "pch.h"
#include "material.h"



std::ostream& operator<<(std::ostream& os, const material& m)
{
    os << "material: color" << const_cast<material&>(m).c();
    if (const_cast<material&>(m).p() != nullptr) const_cast<material&>(m).p()->print();
    else os << "no pattern applied ";
    os << " ambient: " << const_cast<material&>(m).a();
    os << " diffuse: " << const_cast<material&>(m).d();
    os << " specular: " << const_cast<material&>(m).sp();
    os << " shininess: " << const_cast<material&>(m).sh();
    os << " reflective: " << const_cast<material&>(m).reflect() << std::endl;

    return os;
}
