#ifndef RCL_FUNCTIONS
#define RCL_FUNCTIONS

namespace rcl
{

double RandomDouble01();

double RandomDoubleMinMax(double min, double max);

int RandomIntMinMax(int min, int max);

double LinearToGamma(double value);

}
#endif