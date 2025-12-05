#ifndef RCL_PICTURE
#define RCL_PICTURE

#include <string>

#include "vector.hpp"
#include "camera.hpp"

namespace rcl
{

class Picture
{
public:
    Picture(rcl::Camera cam);
    Picture(int width, int height);
    Picture(const char* path);
    Picture(const Picture& other);
    Picture();

    ~Picture();
    
    Picture& operator=(const Picture& other);

    void Import(const char* path);
    void Export(const char* path) const;

    rcl::vec3 GetPixel(const rcl::vec2& uv) const;
    void WritePixel(const int height, const int width, const rcl::vec3& data);

    void GammaCorection();

    int GetWidth() const;
    int GetHeight() const;
    int GetSize() const;
private:
    rcl::vec3* data;
    int width;
    int height;
};

}
#endif