#ifndef RCL_CONSTANTS
#define RCL_CONSTANTS

#include <limits>

namespace rcl
{

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;
const double deegreesToRadians = PI / 180;
const double radiansToDeegrees = 180.0 / PI;

}
#endif