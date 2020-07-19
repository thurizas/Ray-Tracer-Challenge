#pragma once

#include "pch.h"
#include "material.h"



std::ostream& operator<<(std::ostream& os, const material& m)
{
    os << "material: color" << const_cast<material&>(m).c();
    os << " ambient: " << const_cast<material&>(m).a();
    os << " diffuse: " << const_cast<material&>(m).d();
    os << " specular: " << const_cast<material&>(m).sp();
    os << " shininess: " << const_cast<material&>(m).sh() << std::endl;

    return os;
}