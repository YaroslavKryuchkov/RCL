#ifndef RCL_CAMERA
#define RCL_CAMERA

#include "vector.hpp"
#include "ray.hpp"

#include <string>

namespace rcl
{

struct Camera
{
public:
    Camera();

    float aspectRatio;
    unsigned int imageWidth;

    double vfov = 90;

    rcl::vec3 lookFrom = rcl::vec3(0);
    rcl::vec3 lookAt = rcl::vec3(0, 0, -1);
    rcl::vec3 up = rcl::vec3(0, 1, 0);

    double defocusAngle = 0;
    double focusDistance = 10;

    void Initialize();
    
    rcl::Ray GetRay(int i, int j, rcl::vec3 offset = rcl::vec3(0)) const;
    unsigned int GetPixelsTotal() const;
    unsigned int GetImageHeight() const;
    unsigned int GetImageWidth() const;

private:
    unsigned int imageHeight;
    rcl::vec3 pixel00Loc;   
    rcl::vec3 pixelDelta_u;
    rcl::vec3 pixelDelta_v;

    rcl::vec3 u, v, w;

    rcl::vec3 defocusDisk_u;
    rcl::vec3 defocusDisk_v;

    inline rcl::vec3 SampleSquare() const;

    rcl::vec3 DefocusDiskSample() const;
};

}

#endif