#include <fstream>
#include <iostream>

#include "camera.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "constants.hpp"
#include "functions.hpp"

namespace rcl
{

Camera::Camera(){};

rcl::Ray Camera::GetRay(int i, int j, rcl::vec3 offset) const
{
    rcl::vec3 rayOrigin = (defocusAngle <= 0 ? lookFrom : DefocusDiskSample());

    rcl::vec3 pixelCenter = pixel00Loc + ((j + offset.x) * pixelDelta_u) + ((i + offset.y) * pixelDelta_v);
    rcl::vec3 rayDirection = pixelCenter - rayOrigin;

    return rcl::Ray(rayOrigin, rayDirection);
}

unsigned int Camera::GetPixelsTotal() const
{
    return imageHeight * imageWidth;
}

unsigned int Camera::GetImageHeight() const
{
    return imageHeight;
}

unsigned int Camera::GetImageWidth() const
{
    return imageWidth;
}

void Camera::Initialize()
{
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    imageHeight = imageWidth / aspectRatio;

    double theta = vfov * deegreesToRadians;
    double h = std::tan(theta / 2);
    double viewportHeight = 2.0f * h * focusDistance;
    double viewportWidth = viewportHeight * (float(imageWidth)/imageHeight);

    w = (lookFrom - lookAt).Unit();
    u = rcl::Cross(up, w).Unit();
    v = rcl::Cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    rcl::vec3 viewport_u = viewportWidth * u;
    rcl::vec3 viewport_v = viewportHeight * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixelDelta_u = viewport_u / imageWidth;
    pixelDelta_v = viewport_v / imageHeight;

    // Calculate the location of the upper left pixel.
    rcl::vec3 viewportUpperLeft = lookFrom - focusDistance * w - viewport_u/2 - viewport_v/2;
    pixel00Loc = viewportUpperLeft + 0.5 * (pixelDelta_u + pixelDelta_v);

    double defocusRadius = focusDistance * std::tan(deegreesToRadians * defocusAngle / 2);
    defocusDisk_u = v * defocusRadius;
    defocusDisk_v = u * defocusRadius;
}

inline rcl::vec3 Camera::SampleSquare() const
{
    return rcl::vec3(RandomDouble01() - 0.5, RandomDouble01() - 0.5, 0);
}

rcl::vec3 Camera::DefocusDiskSample() const
{
    rcl::vec3 p = rcl::Ray::GetRandomDiskRay();
    return lookFrom + (p.x * defocusDisk_u) + (p.y * defocusDisk_v);
}

}