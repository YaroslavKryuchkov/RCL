#ifndef RCL_VECTOR4
#define RCL_VECTOR4

#include <fstream>

#include "base_vector.hpp"

namespace rcl
{

template <typename T>
struct Vector<4, T>
{
public:
    // Attributes
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};
    union{T w, a;};

    // Constructors
    template <typename T2>
    Vector(T2 scalar = 0);
    template <typename T2, typename T3, typename T4, typename T5>
    Vector(T2 x, T3 y, T4 z, T5 w);
    template <typename T2>
    Vector(Vector<4, T2> const & origin);

    // Methods
    T Length() const override;
    T LengthSquared() const override;
    T MaxComponent() const override;
    Vector<4, T> Unit() const;

    // Unarny operators
    rcl::Vector<4, T> operator+() const;
    rcl::Vector<4, T> operator-() const;

    // Binary operators
    template <typename T2>
    rcl::Vector<4, T>& operator=(rcl::Vector<4, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<4, T>& operator=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<4, T>& operator+=(rcl::Vector<4, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<4, T>& operator+=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<4, T>& operator-=(rcl::Vector<4, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<4, T>& operator-=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<4, T>& operator*=(rcl::Vector<4, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<4, T>& operator*=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<4, T>& operator/=(rcl::Vector<4, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<4, T>& operator/=(T2 const & scalar);

    // Friends
    // Operators
    template <typename T2>
    friend std::ostream& operator<<(std::ostream& os, const Vector<4, T2>& obj);

    template <typename T2, typename T3>
    friend Vector<4, T2> operator+(Vector<4, T2> const & a, Vector<4, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<4, T2> operator+(Vector<4, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<4, T2> operator-(Vector<4, T2> const & a, Vector<4, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<4, T2> operator-(Vector<4, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<4, T2> operator*(Vector<4, T2> const & a, Vector<4, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<4, T2> operator*(Vector<4, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<4, T2> operator/(Vector<4, T2> const & a, Vector<4, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<4, T2> operator/(Vector<4, T2> const & a, T3 const & b);
};

}//namespace rcl

#include "vector4.inl"

#endif