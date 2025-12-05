#include <fstream>
#include <cmath>

namespace rcl
{

template <typename T>
rcl::Vector<2, T>::Vector(T scalar) : x(scalar), y(scalar) {}

template <typename T>
template <typename T2>
rcl::Vector<2, T>::Vector(T2 scalar) : x((T)scalar), y((T)scalar) {}

template <typename T>
template <typename T2, typename T3>
rcl::Vector<2, T>::Vector(T2 x, T3 y) : x((T)x), y((T)y) {}

template <typename T>
template <typename T2>
rcl::Vector<2, T>::Vector(rcl::Vector<2, T2> const & origin)
{
    *this.x = (T)origin.x;
    *this.y = (T)origin.y;
}

template <typename T>
T rcl::Vector<2, T>::Length() const
{
    return sqrt(this->LengthSquared());
}

template <typename T>
T rcl::Vector<2, T>::LengthSquared() const
{
    return x*x + y*y;
}

template <typename T>
T rcl::Vector<2, T>::MaxComponent() const
{
    return x > y ? x : y;
}

template <typename T>
Vector<2, T> rcl::Vector<2, T>::Unit() const
{
    return *this / Length();
}

template <typename T>
rcl::Vector<2, T> rcl::Vector<2, T>::operator+() const
{
    return rcl::Vector<2, T>(*this);
}

template <typename T>
rcl::Vector<2, T> rcl::Vector<2, T>::operator-() const
{
    return rcl::Vector<2, T>(-this->x, -this->y);
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator=(const rcl::Vector<2, T2>& another_vector)
{
    if (this != &another_vector) 
    {
        this->x = another_vector.x;
        this->y = another_vector.y;
    }
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator=(const T2& scalar)
{
    this->x = scalar;
    this->y = scalar;

    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator+=(const rcl::Vector<2, T2>& another_vector)
{
    this->x += (T)another_vector.x;
    this->y += (T)another_vector.y;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator+=(const T2& scalar)
{
    this->x += (T)scalar;
    this->y += (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator-=(const rcl::Vector<2, T2>& another_vector)
{
    this->x -= (T)another_vector.x;
    this->y -= (T)another_vector.y;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator-=(const T2& scalar)
{
    this->x -= (T)scalar;
    this->y -= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator*=(const rcl::Vector<2, T2>& another_vector)
{
    this->x *= (T)another_vector.x;
    this->y *= (T)another_vector.y;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator*=(const T2& scalar)
{
    this->x *= (T)scalar;
    this->y *= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator/=(const rcl::Vector<2, T2>& another_vector)
{
    this->x /= (T)another_vector.x;
    this->y /= (T)another_vector.y;
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<2, T>& rcl::Vector<2, T>::operator/=(const T2& scalar)
{
    this->x /= (T)scalar;
    this->y /= (T)scalar;
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<2, T>& obj)
{
    return (os << obj.x << ' ' << obj.y);
}

template <typename T, typename T2>
Vector<2, T> operator+(Vector<2, T> const & a, Vector<2, T2> const & b)
{
    Vector<2, T> answ(a);
    answ += b;
    return answ;
}
template <typename T, typename T2>
Vector<2, T> operator+(Vector<2, T> const & a, T2 const & b)
{
    Vector<2, T> answ(a);
    answ += b;
    return answ;
}

template <typename T, typename T2>
Vector<2, T> operator-(Vector<2, T> const & a, Vector<2, T2> const & b)
{
    Vector<2, T> answ(a);
    answ -= b;
    return answ;
}
template <typename T, typename T2>
Vector<2, T> operator-(Vector<2, T> const & a, T2 const & b)
{
    Vector<2, T> answ(a);
    answ -= b;
    return answ;
}

template <typename T, typename T2>
Vector<2, T> operator*(Vector<2, T> const & a, Vector<2, T2> const & b)
{
    Vector<2, T> answ(a);
    answ *= b;
    return answ;
}
template <typename T, typename T2>
Vector<2, T> operator*(T2 const & a, Vector<2, T> const & b)
{
    Vector<2, T> answ(a);
    answ *= b;
    return answ;
}

template <typename T, typename T2>
Vector<2, T> operator/(Vector<2, T> const & a, Vector<2, T2> const & b)
{
    Vector<2, T> answ(a);
    answ /= b;
    return answ;
}
template <typename T, typename T2>
Vector<2, T> operator/(Vector<2, T> const & a, T2 const & b)
{
    Vector<2, T> answ(a);
    answ /= b;
    return answ;
}

}//namespace rcl