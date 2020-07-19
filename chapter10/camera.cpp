#include "pch.h"

#include "camera.h"
#include "matrix4x4.h"
#include "canvas.h"


camera::camera() : m_hsize(160), m_vsize(120), m_fov(PI/2.0f), m_transform(matrix4x4::identity())
{ 
    calcPixelSize();
}

camera::camera(int h, int v, float f, matrix4x4 t) : m_hsize(h), m_vsize(v), m_fov(f), m_transform(t)
{ 
    calcPixelSize();
}

camera::camera(const camera& rhs) 
{
    m_hsize = const_cast<camera&>(rhs).hsize();
    m_vsize = const_cast<camera&>(rhs).vsize();
    m_fov = const_cast<camera&>(rhs).fov();
    m_transform = const_cast<camera&>(rhs).t();

    calcPixelSize();
}
camera& camera::operator=(const camera& rhs)
{
    if (*this != rhs)
    {
        this->m_hsize = rhs.m_hsize;
        this->m_vsize = rhs.m_vsize;
        this->m_fov = rhs.m_fov;
        this->m_transform = rhs.m_transform;
        this->calcPixelSize();
    }
    
    return *this;
}
camera::~camera()
{ }

bool camera::operator==(const camera& rhs)
{
    return ((m_hsize == rhs.m_hsize) && (m_vsize == rhs.m_vsize) && (m_fov == rhs.m_fov));
}

bool camera::operator!=(const camera& rhs)
{
    return !(*this == rhs);
}


void camera::calcPixelSize()
{
    float halfView = tan(m_fov / 2.0f);
    float aspect = (float)m_hsize / (float)m_vsize;

    if (aspect >= 1)
    {
        m_halfWidth = halfView;
        m_halfHeight = halfView / aspect;
    }
    else
    {
        m_halfWidth = halfView * aspect;
        m_halfHeight = halfView;

    }

    m_pixelSize = m_halfWidth * 2 / (float)m_hsize;
}

ray camera::rayToPoint(float px, float py)
{
    // off set from edge of canvas to pixel center
    float xoffset = (px + 0.5f)* m_pixelSize;
    float yoffset = (py + 0.5f)* m_pixelSize;

    // coordinates of pixel in world space
    float worldx = m_halfWidth - xoffset;
    float worldy = m_halfHeight - yoffset;

    // transform the pixel location using inverse camera xform
    point pixel = m_transform.inverse() * point(worldx, worldy, -1);
    point origin = m_transform.inverse() * point(0, 0, 0);
    vector direction = pixel - origin;
    direction.normalize();
    ray    r(origin, direction);

    return r;
}

void camera::render(world* w, canvas* i)
{
    for (int y = 0; y < m_vsize - 1; y++)
    {
        for (int x = 0; x < m_hsize - 1; x++)
        {
            ray r = rayToPoint((float)x, (float)y);
            color c = w->intersect(r);
            i->setColor(x, y, c);
        }
        
    }
}






std::ostream& operator<<(std::ostream& os , const camera& c)
{
    os << "camera - horizontal size: " << const_cast<camera&>(c).hsize();
    os << " vertical size: " << const_cast<camera&>(c).vsize();
    os << " field of view: " << const_cast<camera&>(c).fov() << std::endl;
    os << " pixel size is: " << const_cast<camera&>(c).pixelSize() << std::endl;
    os << "transform is: " << std::endl;
    os << const_cast<camera&>(c).t();
    return os;
}