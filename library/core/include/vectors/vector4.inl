#include <fstream>
#include <cmath>

namespace rcl
{

template <typename T>
template <typename T2>
rcl::Vector<4, T>::Vector(T2 scalar) : x((T)scalar), y((T)scalar), z((T)scalar), w((T)scalar) {}

template <typename T>
template <typename T2, typename T3, typename T4, typename T5>
rcl::Vector<4, T>::Vector(T2 x, T3 y, T4 z, T5 w) : x((T)x), y((T)y), z((T)z), w((T)w) {}

template <typename T>
template <typename T2>
rcl::Vector<4, T>::Vector(rcl::Vector<4, T2> const & origin)
{
    *this.x = (T)origin.x;
    *this.y = (T)origin.y;
    *this.z = (T)origin.z;
    *this.w = (T)origin.w;
}

template <typename T>
T rcl::Vector<4, T>::Length() const
{
    return sqrt(this->LengthSquared());
}

template <typename T>
T rcl::Vector<4, T>::LengthSquared() const
{
    return x*x + y*y + z*z + w*w;
}

template <typename T>
T rcl::Vector<4, T>::MaxComponent() const
{
    return std::fmax(x, std::fmax(y, std::fmax(z, w)));
}

template <typename T>
Vector<4, T> rcl::Vector<4, T>::Unit() const
{
    return *this / Length();
}

template <typename T>
rcl::Vector<4, T> rcl::Vector<4, T>::operator+() const
{
    return rcl::Vector<4, T>(*this);
}

template <typename T>
rcl::Vector<4, T> rcl::Vector<4, T>::operator-() const
{
    return rcl::Vector<4, T>(-this->x, -this->y, -this->z, -this->w);
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator=(const rcl::Vector<4, T2>& another_vector)
{
    if (this != &another_vector) 
    {
        this->x = (T)another_vector.x;
        this->y = (T)another_vector.y;
        this->z = (T)another_vector.z;
        this->w = (T)another_vector.w;
    }
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator=(const T2& scalar)
{
    this->x = (T)scalar;
    this->y = (T)scalar;
    this->z = (T)scalar;
    this->w = (T)scalar;

    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator+=(const rcl::Vector<4, T2>& another_vector)
{
    this->x += (T)another_vector.x;
    this->y += (T)another_vector.y;
    this->z += (T)another_vector.z;
    this->w += (T)another_vector.w;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator+=(const T2& scalar)
{
    this->x += (T)scalar;
    this->y += (T)scalar;
    this->z += (T)scalar;
    this->w += (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator-=(const rcl::Vector<4, T2>& another_vector)
{
    this->x -= (T)another_vector.x;
    this->y -= (T)another_vector.y;
    this->z -= (T)another_vector.z;
    this->w -= (T)another_vector.w;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator-=(const T2& scalar)
{
    this->x -= (T)scalar;
    this->y -= (T)scalar;
    this->z -= (T)scalar;
    this->w -= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator*=(const rcl::Vector<4, T2>& another_vector)
{
    this->x *= (T)another_vector.x;
    this->y *= (T)another_vector.y;
    this->z *= (T)another_vector.z;
    this->w *= (T)another_vector.w;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator*=(const T2& scalar)
{
    this->x *= (T)scalar;
    this->y *= (T)scalar;
    this->z *= (T)scalar;
    this->w *= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator/=(const rcl::Vector<4, T2>& another_vector)
{
    this->x /= (T)another_vector.x;
    this->y /= (T)another_vector.y;
    this->z /= (T)another_vector.z;
    this->w /= (T)another_vector.w;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<4, T>& rcl::Vector<4, T>::operator/=(const T2& scalar)
{
    this->x /= (T)scalar;
    this->y /= (T)scalar;
    this->z /= (T)scalar;
    this->w /= (T)scalar;
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<4, T>& obj)
{
    return (os << obj.x << ' ' << obj.y << ' ' << obj.z << ' ' << obj.w);
}

template <typename T, typename T2>
Vector<4, T> operator+(Vector<4, T> const & a, Vector<4, T2> const & b)
{
    Vector<4, T> answ(a);
    answ += b;
    return answ;
}
template <typename T, typename T2>
Vector<4, T> operator+(Vector<4, T> const & a, T2 const & b)
{
    Vector<4, T> answ(a);
    answ += b;
    return answ;
}

template <typename T, typename T2>
Vector<4, T> operator-(Vector<4, T> const & a, Vector<4, T2> const & b)
{
    Vector<4, T> answ(a);
    answ -= b;
    return answ;
}
template <typename T, typename T2>
Vector<4, T> operator-(Vector<4, T> const & a, T2 const & b)
{
    Vector<4, T> answ(a);
    answ -= b;
    return answ;
}

template <typename T, typename T2>
Vector<4, T> operator*(Vector<4, T> const & a, Vector<4, T2> const & b)
{
    Vector<4, T> answ(a);
    answ *= b;
    return answ;
}
template <typename T, typename T2>
Vector<4, T> operator*(Vector<4, T> const & a, T2 const & b)
{
    Vector<4, T> answ(a);
    answ *= b;
    return answ;
}

template <typename T, typename T2>
Vector<4, T> operator/(Vector<4, T> const & a, Vector<4, T2> const & b)
{
    Vector<4, T> answ(a);
    answ /= b;
    return answ;
}
template <typename T, typename T2>
Vector<4, T> operator/(Vector<4, T> const & a, T2 const & b)
{
    Vector<4, T> answ(a);
    answ /= b;
    return answ;
}

}//namespace rcl