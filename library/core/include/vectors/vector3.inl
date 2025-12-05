#include <fstream>
#include <cmath>
#include "../functions.hpp"

namespace rcl
{

template <typename T>
rcl::Vector<3, T>::Vector(T scalar) : x(scalar), y(scalar), z(scalar) {}

template <typename T>
template <typename T2>
rcl::Vector<3, T>::Vector(T2 scalar) : x((T)scalar), y((T)scalar), z((T)scalar) {}

template <typename T>
template <typename T2, typename T3, typename T4>
rcl::Vector<3, T>::Vector(T2 x, T3 y, T4 z) : x((T)x), y((T)y), z((T)z) {}

template <typename T>
template <typename T2>
rcl::Vector<3, T>::Vector(rcl::Vector<3, T2> const & origin)
{
    this->x = (T)origin.x;
    this->y = (T)origin.y;
    this->z = (T)origin.z;
}

template <typename T>
T rcl::Vector<3, T>::Length() const
{
    return sqrt(this->LengthSquared());
}

template <typename T>
T rcl::Vector<3, T>::LengthSquared() const
{
    return x*x + y*y + z*z;
}

template <typename T>
T rcl::Vector<3, T>::MaxComponent() const
{
    return std::fmax(x, std::fmax(y, z));
}

template <typename T>
Vector<3, T> rcl::Vector<3, T>::Unit() const
{
    return *this / Length();
}

template <typename T>
rcl::Vector<3, T> rcl::Vector<3, T>::RandomUnitVector()
{
    return RandomVector(-1, 1).Unit();
}

template <typename T>
rcl::Vector<3, T> rcl::Vector<3, T>::RandomVector(T min, T max)
{
    return rcl::Vector<3, T>(RandomDoubleMinMax(min, max), RandomDoubleMinMax(min, max), RandomDoubleMinMax(min, max));
}

template <typename T>
rcl::Vector<3, T> rcl::Vector<3, T>::operator+() const
{
    return rcl::Vector<3, T>(*this);
}

template <typename T>
rcl::Vector<3, T> rcl::Vector<3, T>::operator-() const
{
    return rcl::Vector<3, T>(-this->x, -this->y, -this->z);
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator=(const rcl::Vector<3, T2>& another_vector)
{
    if (*this != another_vector) 
    {
        this->x = another_vector.x;
        this->y = another_vector.y;
        this->z = another_vector.z;
    }
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator=(const T2& scalar)
{
    this->x = scalar;
    this->y = scalar;
    this->z = scalar;

    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator+=(const rcl::Vector<3, T2>& another_vector)
{
    this->x += (T)another_vector.x;
    this->y += (T)another_vector.y;
    this->z += (T)another_vector.z;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator+=(const T2& scalar)
{
    this->x += (T)scalar;
    this->y += (T)scalar;
    this->z += (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator-=(const rcl::Vector<3, T2>& another_vector)
{
    this->x -= (T)another_vector.x;
    this->y -= (T)another_vector.y;
    this->z -= (T)another_vector.z;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator-=(const T2& scalar)
{
    this->x -= (T)scalar;
    this->y -= (T)scalar;
    this->z -= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator*=(const rcl::Vector<3, T2>& another_vector)
{
    this->x *= (T)another_vector.x;
    this->y *= (T)another_vector.y;
    this->z *= (T)another_vector.z;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator*=(const T2& scalar)
{
    this->x *= (T)scalar;
    this->y *= (T)scalar;
    this->z *= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator/=(const rcl::Vector<3, T2>& another_vector)
{
    this->x /= (T)another_vector.x;
    this->y /= (T)another_vector.y;
    this->z /= (T)another_vector.z;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<3, T>& rcl::Vector<3, T>::operator/=(const T2& scalar)
{
    this->x /= (T)scalar;
    this->y /= (T)scalar;
    this->z /= (T)scalar;
    return *this;
}

template <typename T>
T rcl::Vector<3, T>::operator[](int index) const
{
    if(index == 0) return x;
    if(index == 1) return y;
    return z;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<3, T>& obj)
{
    return (os << obj.x << ' ' << obj.y << ' ' << obj.z);
}

template <typename T, typename T2>
Vector<3, T> operator+(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    Vector<3, T> answ(a);
    answ += b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator+(Vector<3, T> const & a, T2 const & b)
{
    Vector<3, T> answ(a);
    answ += b;
    return answ;
}

template <typename T, typename T2>
Vector<3, T> operator-(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    Vector<3, T> answ(a);
    answ -= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator-(Vector<3, T> const & a, T2 const & b)
{
    Vector<3, T> answ(a);
    answ -= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator-(T2 const & a, Vector<3, T> const & b)
{
    Vector<3, T> answ(a);
    answ -= b;
    return answ;
}

template <typename T, typename T2>
Vector<3, T> operator*(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    Vector<3, T> answ(a);
    answ *= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator*(Vector<3, T> const & a, T2 const & b)
{
    Vector<3, T> answ(a);
    answ *= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator*(T2 const & b, Vector<3, T> const & a)
{
    Vector<3, T> answ(a);
    answ *= b;
    return answ;
}

template <typename T, typename T2>
Vector<3, T> operator/(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    Vector<3, T> answ(a);
    answ /= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator/(Vector<3, T> const & a, T2 const & b)
{
    Vector<3, T> answ(a);
    answ /= b;
    return answ;
}
template <typename T, typename T2>
Vector<3, T> operator/(T2 const & b, Vector<3, T> const & a)
{
    Vector<3, T> answ(a);
    answ /= b;
    return answ;
}
    
template <typename T>
bool operator==(Vector<3, T> const & a, Vector<3, T> const & b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

template <typename T, typename T2>
bool operator!=(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    return (a.x != b.x) && (a.y != b.y) && (a.z != b.z);
}

template <typename T, typename T2>
inline T Dot(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T, typename T2>
inline Vector<3, T> Cross(Vector<3, T> const & a, Vector<3, T2> const & b)
{
    return Vector<3, T>(a.y * b.z - a.z * b.y , 
                        a.z * b.x - a.x * b.z , 
                        a.x * b.y - a.y * b.x);
}

template <typename T, typename T2>
inline Vector<3, T> Reflect(Vector<3, T> const & v, Vector<3, T2> const & n)
{
    return v - 2 * rcl::Dot(v, n) * n;
}

template <typename T, typename T2>
inline Vector<3, T> Refract(Vector<3, T> const & uv, Vector<3, T2> const & n, double etaiOverEtat)
{
    double cosTheta = std::fmin(rcl::Dot(-uv, n), 1.0);
    Vector<3, T> rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vector<3, T> rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
    return rOutPerp + rOutParallel;
}

}//namespace rcl