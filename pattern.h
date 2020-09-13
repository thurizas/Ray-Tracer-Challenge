#pragma once

#include "pch.h"


#include "color.h"
#include "point.h"
#include "matrix4x4.h"
#include "object.h"

const color WHITE = color(1.0, 1.0, 1.0);
const color BLACK = color(0.0, 0.0, 0.0);
const color RED = color(1.0, 0.0, 0.0);
const color YELLOW = color(1.0, 1.0, 0.0);

class object;

class pattern
{
public:
    enum {TEST=0,STRIPED, GRADIENT, RING, CHECKERED };
    pattern(color a, color b, int t) : m_type(t), m_a(a), m_b(b), m_xform(matrix4x4::identity())
    { }
    virtual ~pattern() { }

    color a() { return m_a; }
    void a(color c) { m_a = c; }

    color b() { return m_b; }
    void b(color c) { m_b = c; }

    matrix4x4 xform() { return m_xform; }
    void xform(matrix4x4 m) { m_xform = m_xform * m; }

    std::string type(); 
    virtual void print() const;

    virtual color stripeAt(point, object* o = nullptr) = 0;
 

protected:
    int   m_type;
    color m_a;
    color m_b;

    matrix4x4 m_xform;
};





//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class defPattern : public pattern
{
public:
    defPattern(color a=WHITE, color b=BLACK) : pattern(a, b, TEST)
    { }
    ~defPattern() { }
    virtual color stripeAt(point pt, object* o = nullptr);
};

class stripePattern : public pattern
{
public:
    stripePattern(color a, color b) : pattern(a, b, STRIPED)
    { }
    ~stripePattern() { }

    virtual color stripeAt(point pt, object* o = nullptr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class gradientPattern : public pattern
{
public:
    gradientPattern(color a, color b): pattern(a, b, GRADIENT)
    { }
    ~gradientPattern( ) { }

    virtual color stripeAt(point pt, object* o = nullptr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class radialPattern : public pattern
{
public:
    radialPattern(color a, color b) : pattern(a, b, GRADIENT)
    { }
    ~radialPattern() { }

    virtual color stripeAt(point pt, object* o = nullptr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class checkeredPattern : public pattern
{
public:
    checkeredPattern(color a, color b) : pattern(a, b, GRADIENT)
    { }
    ~checkeredPattern() { }

    virtual color stripeAt(point pt, object* o = nullptr);
};
