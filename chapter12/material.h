#pragma once
#include <iostream>
#include "color.h"
#include "pattern.h"

class material
{
public:
    material() : m_color(color(1.0f, 1.0f, 1.0f)), m_pattern(nullptr), m_ambient(0.1f), m_diffuse(0.9f), m_specular(0.9f), 
                 m_shininess(200.0f), m_reflective(0.0f), m_transparency(0.0), m_refractiveIndex(1.0) { }
    material(color c,  float a, float d, float sp, float sh, float re, pattern* p = nullptr) : m_color(c), m_pattern(p), m_ambient(a), m_diffuse(d), m_specular(sp), m_shininess(sh), m_reflective(re) { }

    color c() { return m_color; }
    void c(color c) { m_color = c; }
    pattern* p() { return m_pattern; }
    void p(pattern* p) { m_pattern = p; }
    float a() { return m_ambient; }
    void a(float a) { m_ambient = a; }
    float d() { return m_diffuse; }
    void d(float d) { m_diffuse = d; }
    float sp() { return m_specular; }
    void sp(float s) { m_specular = s; }
    float sh() { return m_shininess; }
    void sh(float s) { m_shininess = s; }
    float reflect() { return m_reflective; }
    void reflect(float r) { m_reflective = r; }
    float transpar() { return m_transparency; }
    void transpar(float t) { m_transparency = t; }
    float refractIndex() { return m_refractiveIndex; }
    void refractIndex(float ndx) { m_refractiveIndex = ndx; }


private:
    color    m_color;
    pattern* m_pattern;
    float    m_ambient;
    float    m_diffuse;
    float    m_specular;
    float    m_shininess;
    float    m_reflective;
    float    m_transparency;
    float    m_refractiveIndex;
};


std::ostream& operator<<(std::ostream& os, const material& m);