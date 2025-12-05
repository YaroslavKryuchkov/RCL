#ifndef RCL_PICTURE_TEXTURE
#define RCL_PICTURE_TEXTURE

#include <memory>

#include "texture.hpp"
#include "picture.hpp"

namespace rcl
{

class PictureTexture : public Texture
{
public:
    PictureTexture(const char* path);

    color GetColor(const uv& uv) const override;

    void SetPicture(std::shared_ptr<Picture> copy);
    void SetPicture(const char* path);
private:
    std::shared_ptr<Picture> pic;
};

}
#endif