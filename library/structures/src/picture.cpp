#include "picture.hpp"

#include <iostream>
#include <cstring>
#include <algorithm>

#include "functions.hpp"
#include "pictures_workers.hpp"

namespace rcl
{
Picture::Picture() : data(nullptr), width(0), height(0) {}

Picture::Picture(rcl::Camera cam) : data(nullptr), width(cam.GetImageWidth()), height(cam.GetImageHeight())
{
    data = new rcl::vec3[width * height];
}

Picture::Picture(int width, int height) : data(nullptr), width(width), height(height)
{
    if(width < 0 || height < 0)
    {
        std::cout << "Wrong sizes height:" << height << " width:" << width << " shoud be more that 0" << std::endl;
        return;
    }
    data = new rcl::vec3[width * height];
}

Picture::Picture(const char* path) : data(nullptr), width(0), height(0)
{
    Import(path);
}

Picture::Picture(const Picture& other) : data(nullptr), width(other.width), height(other.height) 
{
    if (other.data) 
    {
        data = new rcl::vec3[width * height];
        std::memcpy(data, other.data, width * height * sizeof(rcl::vec3));
    }
}

Picture& Picture::operator=(const Picture& other) 
{
    if (this != &other) 
    {
        delete[] data;
        width = other.width;
        height = other.height;
        if (other.data) 
        {
            data = new rcl::vec3[width * height];
            std::memcpy(data, other.data, width * height * sizeof(rcl::vec3));
        } 
        else 
        {
            data = nullptr;
        }
    }
    return *this;
}

Picture::~Picture()
{
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
}

void Picture::Import(const char* path)
{
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
    
    std::string ext = strrchr(path, '.');
    if (!ext.size())
    {
        std::cerr << "Error: No file extension found in " << path << std::endl;
        return;
    }
    
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (strcmp(ext.c_str(), ".png") == 0)
    {
        ImportPNG(path, width, height, data);
    }
    else if (strcmp(ext.c_str(), ".ppm") == 0)
    {
        ImportPPM(path, width, height, data);
    }
    else
    {
        std::cerr << "Error: Unsupported file format " << ext << std::endl;
    }
}

void Picture::Export(const char* path) const
{
    if (!data)
    {
        std::cerr << "Error: No image data to export" << std::endl;
        return;
    }
    
    std::string ext = strrchr(path, '.');
    if (!ext.size())
    {
        std::cerr << "Error: No file extension found in " << path << std::endl;
        return;
    }

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (strcmp(ext.c_str(), ".png") == 0)
    {
        ExportPNG(path, width, height, data);
    }
    else if (strcmp(ext.c_str(), ".ppm") == 0)
    {
        ExportPPM(path, width, height, data);
    }
    else
    {
        std::cerr << "Error: Unsupported file format " << ext << std::endl;
    }
}

rcl::vec3 Picture::GetPixel(const rcl::vec2& uv) const
{
    if (!data || width == 0 || height == 0)
    {
        return rcl::vec3(0.0f, 0.0f, 0.0f);
    }
    
    // Clamp UV coordinates to [0, 1] range
    float u = std::max(0.0f, std::min(1.0f, uv.x));
    float v = std::max(0.0f, std::min(1.0f, uv.y));
    
    // Convert UV to pixel coordinates
    // Note: V is flipped because image coordinates typically start from top-left
    int x = static_cast<int>(u * (width - 1));
    int y = static_cast<int>((1.0f - v) * (height - 1));
    
    // Ensure coordinates are within bounds
    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    
    // Return pixel color
    int index = y * width + x;
    return data[index];
}
    
void Picture::WritePixel(const int h, const int w, const rcl::vec3& d)
{
    if (h < 0 || h >= height || w < 0 || w >= width) return; // Add this
    data[h * width + w] = d;
}

void Picture::GammaCorection()
{
    for(int i = 0; i < height * width; i++)
    {
        data[i].r = rcl::LinearToGamma(data[i].r);
        data[i].g = rcl::LinearToGamma(data[i].g);
        data[i].b = rcl::LinearToGamma(data[i].b);
    }
}

int Picture::GetWidth() const
{
    return width;
}
int Picture::GetHeight() const
{
    return height;
}
int Picture::GetSize() const
{
    return height * width;
}

}