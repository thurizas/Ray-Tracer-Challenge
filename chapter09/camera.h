#pragma once

#include "pch.h"
#include "matrix4x4.h"
#include "ray.h"
#include "world.h"
#include "canvas.h"

#include <iostream>

class camera
{
public:
    camera();
    camera(int, int, float, matrix4x4);
    camera(const camera& rhs);
    camera& operator=(const camera& rhs);
    ~camera();

    int hsize() { return m_hsize; }
    void hsize(int s) { m_hsize = s; calcPixelSize(); }
    int vsize() { return m_vsize; }
    void vsize(int s) { m_vsize = s; calcPixelSize();}
    float fov() { return m_fov; }
    void fov(float f) { m_fov = f; }
    float pixelSize() { return m_pixelSize; }  // no setter as this is a derived quanity.
    matrix4x4 t() { return m_transform; }
    void t(matrix4x4 t1) { m_transform = t1; }

    bool operator==(const camera& rhs);
    bool operator!=(const camera& rhs);

    ray  rayToPoint(float px, float py);
    void render(world* w, canvas* i);

private:
    int       m_hsize;
    int       m_vsize;
    float     m_fov;            // field of view;
    float     m_halfHeight;
    float     m_halfWidth;
    float     m_pixelSize;
    matrix4x4 m_transform;

    void calcPixelSize();
};

std::ostream& operator<<(std::ostream&, const camera&);
