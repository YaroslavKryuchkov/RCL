#include <fstream>
#include <cmath>

namespace rcl
{

template <typename T>
template <typename T2>
rcl::Vector<1, T>::Vector(T2 x) : x((T)x) {}

template <typename T>
template <typename T2>
rcl::Vector<1, T>::Vector(rcl::Vector<1, T2> const & origin)
{
    this.x = (T)origin.x;
}

template <typename T>
T rcl::Vector<1, T>::Length() const
{
    return sqrt(this->LengthSquared());
}

template <typename T>
T rcl::Vector<1, T>::LengthSquared() const
{
    return x*x;
}

template <typename T>
T rcl::Vector<1, T>::MaxComponent() const
{
    return x;
}

template <typename T>
Vector<1, T> rcl::Vector<1, T>::Unit() const
{
    return (T)1;
}

template <typename T>
rcl::Vector<1, T> rcl::Vector<1, T>::operator+() const
{
    return rcl::Vector<1, T>(*this);
}

template <typename T>
rcl::Vector<1, T> rcl::Vector<1, T>::operator-() const
{
    return rcl::Vector<1, T>(-this->x);
}

template <typename T>
template <typename T2>
rcl::Vector<1, T>& rcl::Vector<1, T>::operator=(const rcl::Vector<1, T2>& another_vector)
{
    if (this != &another_vector) 
    {
        this->x = another_vector.x;
    }
    return *this;
}

template <typename T>
template <typename T2>
rcl::Vector<1, T>& rcl::Vector<1, T>::operator=(const T2& scalar)
{
    this->x = scalar;

    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator+=(const rcl::Vector<1, T2>& another_vector)
{
    this->x += (T)another_vector.x;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator+=(const T2& scalar)
{
    this->x += (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator-=(const rcl::Vector<1, T2>& another_vector)
{
    this->x -= (T)another_vector.x;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator-=(const T2& scalar)
{
    this->x -= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator*=(const rcl::Vector<1, T2>& another_vector)
{
    this->x *= (T)another_vector.x;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator*=(const T2& scalar)
{
    this->x *= (T)scalar;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator/=(const rcl::Vector<1, T2>& another_vector)
{
    this->x /= (T)another_vector.x;
    return *this;
}

template <typename T>
template <typename T2>
inline rcl::Vector<1, T>& rcl::Vector<1, T>::operator/=(const T2& scalar)
{
    this->x /= (T)scalar;
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<1, T>& obj)
{
    return (os << obj.x);
}

template <typename T, typename T2>
Vector<1, T> operator+(Vector<1, T> const & a, Vector<1, T2> const & b)
{
    Vector<1, T> answ(a);
    answ += b;
    return answ;
}
template <typename T, typename T2>
Vector<1, T> operator+(Vector<1, T> const & a, T2 const & b)
{
    Vector<1, T> answ(a);
    answ += b;
    return answ;
}

template <typename T, typename T2>
Vector<1, T> operator-(Vector<1, T> const & a, Vector<1, T2> const & b)
{
    Vector<1, T> answ(a);
    answ -= b;
    return answ;
}
template <typename T, typename T2>
Vector<1, T> operator-(Vector<1, T> const & a, T2 const & b)
{
    Vector<1, T> answ(a);
    answ -= b;
    return answ;
}

template <typename T, typename T2>
Vector<1, T> operator*(Vector<1, T> const & a, Vector<1, T2> const & b)
{
    Vector<1, T> answ(a);
    answ *= b;
    return answ;
}
template <typename T, typename T2>
Vector<1, T> operator*(Vector<1, T> const & a, T2 const & b)
{
    Vector<1, T> answ(a);
    answ *= b;
    return answ;
}

template <typename T, typename T2>
Vector<1, T> operator/(Vector<1, T> const & a, Vector<1, T2> const & b)
{
    Vector<1, T> answ(a);
    answ /= b;
    return answ;
}
template <typename T, typename T2>
Vector<1, T> operator/(Vector<1, T> const & a, T2 const & b)
{
    Vector<1, T> answ(a);
    answ /= b;
    return answ;
}

}//namespace rcl