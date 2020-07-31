#include <iostream>
#include <string>
#include "object.h"
#include "pattern.h"

const char* types[] = { "striped  ", "gradient ", "ring     ", "checkered" };

std::string pattern::type()
{ 
    return types[m_type]; 
}

void pattern::print() const
{
    std::cout << "pattern is: " << types[m_type] << "color one: " << m_a << " color two:" << m_b << std::endl;
    std::cout << "transformation applied is: " << m_xform << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of striped pattern
color stripePattern::stripeAt(point pt, object* o)
{
    point objSpace;

    if (o != nullptr)
        objSpace = (o->xform().inverse())*pt;
    else
        objSpace = pt;

    point patSpace = (xform().inverse())*objSpace;

    int x = (int)floor(patSpace.x());

    if (x % 2 == 0) return m_a;
    else return m_b;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of gradient pattern
color gradientPattern::stripeAt(point pt, object* o)
{
    point objSpace;

    if (o != nullptr)
        objSpace = (o->xform().inverse())*pt;
    else
        objSpace = pt;

    point patSpace = (xform().inverse())*objSpace;

    color cl = m_a + (m_b - m_a)*(patSpace.x() - (int)floor(patSpace.x()));
    return cl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of radial pattern
color radialPattern::stripeAt(point pt, object* o)
{
    point objSpace;

    if (o != nullptr)
        objSpace = (o->xform().inverse())*pt;
    else
        objSpace = pt;

    point patSpace = (xform().inverse())*objSpace;

    float u = patSpace.x();
    float v = patSpace.z();
    int r = (int)floor(sqrt(u*u + v * v));

    if (r % 2 == 0) return m_a;
    else return m_b;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of checkered board pattern
color checkeredPattern::stripeAt(point pt, object* o)
{
    point objSpace;

    if (o != nullptr)
        objSpace = (o->xform().inverse())*pt;
    else
        objSpace = pt;

    point patSpace = (xform().inverse())*objSpace;

    int u = (int)floor(patSpace.x());
    int v = (int)floor(patSpace.y());
    int w = (int)floor(patSpace.z());

    if ((u+v+w) % 2 == 0) return m_a;
    else return m_b;

}



