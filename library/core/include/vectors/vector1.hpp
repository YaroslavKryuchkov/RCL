#ifndef RCL_VECTOR1
#define RCL_VECTOR1

#include <fstream>

#include "base_vector.hpp"

namespace rcl
{

template <typename T>
struct Vector<1, T>
{
public:
    // Attributes
    union{T x, r;};

    // Constructors
    template <typename T2>
    Vector(T2 x = 0);
    template <typename T2>
    Vector(Vector<1, T2> const & origin);

    // Methods
    T Length() const override;
    T LengthSquared() const override;
    T MaxComponent() const override;
    Vector<1, T> Unit() const;

    // Unarny operators
    rcl::Vector<1, T> operator+() const;
    rcl::Vector<1, T> operator-() const;

    // Binary operators
    template <typename T2>
    inline rcl::Vector<1, T>& operator=(rcl::Vector<1, T2> const & another_vector);
    template <typename T2>
    inline rcl::Vector<1, T>& operator=(T2 const & scalar);

    template <typename T2>
    inline rcl::Vector<1, T>& operator+=(rcl::Vector<1, T2> const & another_vector);
    template <typename T2>
    inline rcl::Vector<1, T>& operator+=(T2 const & scalar);

    template <typename T2>
    inline rcl::Vector<1, T>& operator-=(rcl::Vector<1, T2> const & another_vector);
    template <typename T2>
    inline rcl::Vector<1, T>& operator-=(T2 const & scalar);

    template <typename T2>
    inline rcl::Vector<1, T>& operator*=(rcl::Vector<1, T2> const & another_vector);
    template <typename T2>
    inline rcl::Vector<1, T>& operator*=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<1, T>& operator/=(rcl::Vector<1, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<1, T>& operator/=(T2 const & scalar);

    // Friends
    // Operators
    template <typename T2>
    friend std::ostream& operator<<(std::ostream& os, const Vector<1, T2>& obj);

    template <typename T2, typename T3>
    friend Vector<1, T2> operator+(Vector<1, T2> const & a, Vector<1, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<1, T2> operator+(Vector<1, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<1, T2> operator-(Vector<1, T2> const & a, Vector<1, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<1, T2> operator-(Vector<1, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<1, T2> operator*(Vector<1, T2> const & a, Vector<1, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<1, T2> operator*(Vector<1, T2> const & a, T3 const & b);

    template <typename T2, typename T3>
    friend Vector<1, T2> operator/(Vector<1, T2> const & a, Vector<1, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<1, T2> operator/(Vector<1, T2> const & a, T3 const & b);
};

}//namespace rcl

#include "vector1.inl"

#endif