#pragma once

#include <iostream>
#include <map>


#include "sphere.h"
#include "world.h"

std::ostream& operator<<(std::ostream&, const std::pair<int, float>);
sphere* glassSphere();
world* defaultWorld();
