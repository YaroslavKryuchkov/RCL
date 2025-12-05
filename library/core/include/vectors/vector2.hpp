#ifndef RCL_VECTOR2
#define RCL_VECTOR2

#include <fstream>

#include "base_vector.hpp"

namespace rcl
{

template <typename T>
struct Vector<2, T>
{
public:
    // Attributes
    union{T x, r, u;};
    union{T y, g, v;};

    // Constructors
    Vector(T scalar = 0);
    template <typename T2>
    Vector(T2 scalar = 0);
    template <typename T2, typename T3>
    Vector(T2 x, T3 y);
    template <typename T2>
    Vector(Vector<2, T2> const & origin);

    // Methods
    T Length() const;
    T LengthSquared() const;
    T MaxComponent() const;
    Vector<2, T> Unit() const;

    // Unarny operators
    rcl::Vector<2, T> operator+() const;
    rcl::Vector<2, T> operator-() const;

    // Binary operators
    template <typename T2>
    rcl::Vector<2, T>& operator=(rcl::Vector<2, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<2, T>& operator=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<2, T>& operator+=(rcl::Vector<2, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<2, T>& operator+=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<2, T>& operator-=(rcl::Vector<2, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<2, T>& operator-=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<2, T>& operator*=(rcl::Vector<2, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<2, T>& operator*=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<2, T>& operator/=(rcl::Vector<2, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<2, T>& operator/=(T2 const & scalar);

    // Friends
    // Operators
    template <typename T2>
    friend std::ostream& operator<<(std::ostream& os, const Vector<2, T2>& obj);

    template <typename T2, typename T3>
    friend Vector<2, T2> operator+(Vector<2, T2> const & a, Vector<2, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<2, T2> operator+(Vector<2, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<2, T2> operator-(Vector<2, T2> const & a, Vector<2, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<2, T2> operator-(Vector<2, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<2, T2> operator*(Vector<2, T2> const & a, Vector<2, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<2, T2> operator*(Vector<2, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<2, T2> operator/(Vector<2, T2> const & a, Vector<2, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<2, T2> operator/(Vector<2, T2> const & a, T3 const & b);
};

}//namespace rcl

#include "vector2.inl"

#endif