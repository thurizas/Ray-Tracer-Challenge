#pragma once

#include "matrix4x4.h"


matrix4x4 translation(float, float, float);
matrix4x4 scale(float, float, float);
matrix4x4 shear(float, float, float, float, float, float);
matrix4x4 rotation_x(float);
matrix4x4 rotation_y(float);
matrix4x4 rotation_z(float );
matrix4x4 viewTransform(point, point, vector);
