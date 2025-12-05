#ifndef RCL_BASE_VECTOR
#define RCL_BASE_VECTOR

namespace rcl
{

template <unsigned int size, typename T>
struct Vector
{
    unsigned int Size() const
    {
        return size;
    }

    Vector<size, T> Unit() const
    {
        return *this / Length();
    }

    T Length() const;
    T LengthSquared() const;
    T MaxComponent() const;
};

}
#endif