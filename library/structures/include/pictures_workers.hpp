#ifndef RCL_PICTURES_WORKERS
#define RCL_PICTURES_WORKERS

#include "vector.hpp"

namespace rcl
{
    void ImportPNG(const char* path, int& width, int& height, rcl::vec3*& data);
    void ExportPNG(const char* path, int width, int height, const rcl::vec3* const & data);

    void ImportPPM(const char* path, int& width, int& height, rcl::vec3*& data);
    void ExportPPM(const char* path, int width, int height, const rcl::vec3* const & data);
}

#endif