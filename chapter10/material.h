#pragma once
#include <iostream>
#include "color.h"
#include "pattern.h"

class material
{
public:
    material() : m_color(color(1.0f, 1.0f, 1.0f)), m_pattern(nullptr), m_ambient(0.1f), m_diffuse(0.9f), m_specular(0.9f), m_shininess(200.0f) { }
    material(color c,  float a, float d, float sp, float sh, pattern* p = nullptr) : m_color(c), m_pattern(p), m_ambient(a), m_diffuse(d), m_specular(sp), m_shininess(sh) { }

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


private:
    color    m_color;
    pattern* m_pattern;
    float    m_ambient;
    float    m_diffuse;
    float    m_specular;
    float    m_shininess;
};


std::ostream& operator<<(std::ostream& os, const material& m);