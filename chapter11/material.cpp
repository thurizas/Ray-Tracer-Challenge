#include "pch.h"
#include "material.h"



std::ostream& operator<<(std::ostream& os, const material& m)
{
    os << "material: color" << const_cast<material&>(m).c() << std::endl;
    if (const_cast<material&>(m).p() != nullptr) const_cast<material&>(m).p()->print();
    else os << "no pattern applied ";
    os << "\n ambient: " << const_cast<material&>(m).a();
    os << " diffuse: " << const_cast<material&>(m).d();
    os << " specular: " << const_cast<material&>(m).sp();
    os << " shininess: " << const_cast<material&>(m).sh() << std::endl;
    os << " reflective: " << const_cast<material&>(m).reflect();
    os << " transparency: " << const_cast<material&>(m).transpar();
    os << " refractive index: " << const_cast<material&>(m).refractIndex() << std::endl;

    return os;
}
