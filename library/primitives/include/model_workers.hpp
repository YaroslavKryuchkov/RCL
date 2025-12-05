#ifndef RCL_MODEL_WORKERS
#define RCL_MODEL_WORKERS

#include "hittable_list.hpp"

namespace rcl
{
    void ImportOBJ(const char* path, HittableList& triangles);
}

#endif