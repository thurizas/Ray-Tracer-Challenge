#include "pch.h"

#include "transformations.h"

#include "matrix4x4.h"
#include "vector.h"
#include "point.h"

matrix4x4 translation(float x, float y, float z)
{
    matrix4x4 xform = matrix4x4::identity();
    xform.setElement(0, 3, x);
    xform.setElement(1, 3, y);
    xform.setElement(2, 3, z);

    return xform;
}

matrix4x4 scale(float sx, float sy, float sz)
{
    matrix4x4 xform = matrix4x4::identity();

    xform.setElement(0, 0, sx);
    xform.setElement(1, 1, sy);
    xform.setElement(2, 2, sz);

    return xform;
}

matrix4x4 shear(float x_y, float x_z, float y_x, float y_z, float z_x, float z_y)
{
    matrix4x4 xform = matrix4x4::identity();

    xform.setElement(0, 1, x_y);
    xform.setElement(0, 2, x_z);

    xform.setElement(1, 0, y_x);
    xform.setElement(1, 2, y_z);

    xform.setElement(2, 0, z_x);
    xform.setElement(2, 1, z_y);

    return xform;
}

matrix4x4 rotation_x(float theta)
{
    matrix4x4 xform = matrix4x4::identity();
    float     angle = theta * DEG2RAD;           // angle in radians
    float     s = sin(angle);
    float     c = cos(angle);

    xform.setElement(1, 1, c);
    xform.setElement(1, 2, -s);
    xform.setElement(2, 1, s);
    xform.setElement(2, 2, c);

    return xform;
}

matrix4x4 rotation_y(float theta)
{
    matrix4x4 xform = matrix4x4::identity();
    float     angle = theta * DEG2RAD;           // angle in radians
    float     s = sin(angle);
    float     c = cos(angle);

    xform.setElement(0, 0, c);
    xform.setElement(0, 2, s);
    xform.setElement(2, 0, -s);
    xform.setElement(2, 2, c);
    return xform;
}

matrix4x4 rotation_z(float theta)
{
    matrix4x4 xform = matrix4x4::identity();
    float     angle = theta * DEG2RAD;           // angle in radians
    float     s = sin(angle);
    float     c = cos(angle);

    xform.setElement(0, 0, c);
    xform.setElement(0, 1, -s);
    xform.setElement(1, 0, s);
    xform.setElement(1, 1, c);

    return xform;
}

matrix4x4 viewTransform(point from, point to, vector up)
{
    // get forward vector...
    vector forward = (to - from);
    forward.normalize();
    // get approximate up vector...
    vector upn = up;
    upn.normalize();
    // get left vector...
    vector left = forward.cross(upn);
    // get true up vector ...
    vector trueup = left.cross(forward);

    matrix4x4 xform(left.x()   , left.y()    , left.z()    , 0,
                    trueup.x() , trueup.y()  , trueup.z()  , 0,
                   -forward.x(), -forward.y(), -forward.z(), 0,
                      0        , 0           , 0           , 1);

    return xform * translation(-from.x(), -from.y(), -from.z());

}