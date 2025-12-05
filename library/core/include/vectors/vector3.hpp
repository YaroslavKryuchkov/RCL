#ifndef RCL_VECTOR3
#define RCL_VECTOR3

#include <fstream>

#include "base_vector.hpp"

namespace rcl
{

template <typename T>
struct Vector<3, T>
{
public:
    // Attributes
    union{T x, r;};
    union{T y, g;};
    union{T z, b;};

    // Constructors
    Vector(T scalar = 0);
    template <typename T2>
    Vector(T2 scalar = 0);
    template <typename T2, typename T3, typename T4>
    Vector(T2 x, T3 y, T4 z);
    template <typename T2>
    Vector(Vector<3, T2> const & origin);

    // Methods
    T Length() const;
    T LengthSquared() const;
    T MaxComponent() const;
    Vector<3, T> Unit() const;
    
    static rcl::Vector<3, T> RandomUnitVector();
    static rcl::Vector<3, T> RandomVector(T min, T max);

    // Unarny operators
    rcl::Vector<3, T> operator+() const;
    rcl::Vector<3, T> operator-() const;

    // Binary operators
    template <typename T2>
    rcl::Vector<3, T>& operator=(rcl::Vector<3, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<3, T>& operator=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<3, T>& operator+=(rcl::Vector<3, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<3, T>& operator+=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<3, T>& operator-=(rcl::Vector<3, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<3, T>& operator-=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<3, T>& operator*=(rcl::Vector<3, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<3, T>& operator*=(T2 const & scalar);

    template <typename T2>
    rcl::Vector<3, T>& operator/=(rcl::Vector<3, T2> const & another_vector);
    template <typename T2>
    rcl::Vector<3, T>& operator/=(T2 const & scalar);
    
    T operator[](int index) const;

    // Friends
    // Operators
    template <typename T2>
    friend std::ostream& operator<<(std::ostream& os, const Vector<3, T2>& obj);

    template <typename T2, typename T3>
    friend Vector<3, T2> operator+(Vector<3, T2> const & a, Vector<3, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator+(Vector<3, T2> const & a, T3 const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator+(T3 const & b, Vector<3, T2> const & a);

    template <typename T2, typename T3>
    friend Vector<3, T2> operator-(Vector<3, T2> const & a, Vector<3, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator-(Vector<3, T2> const & a, T3 const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator-(T3 const & b, Vector<3, T2> const & a);

    template <typename T2, typename T3>
    friend Vector<3, T2> operator*(Vector<3, T2> const & a, Vector<3, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator*(Vector<3, T2> const & a, T3 const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator*(T3 const & b, Vector<3, T2> const & a);

    template <typename T2, typename T3>
    friend Vector<3, T2> operator/(Vector<3, T2> const & a, Vector<3, T3> const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator/(Vector<3, T2> const & a, T3 const & b);
    template <typename T2, typename T3>
    friend Vector<3, T2> operator/(T3 const & b, Vector<3, T2> const & a);
    
    template <typename T2>
    friend bool operator==(Vector<3, T2> const & a, Vector<3, T2> const & b);

    template <typename T2, typename T3>
    friend bool operator!=(Vector<3, T2> const & a, Vector<3, T3> const & b);

    template <typename T2, typename T3>
    friend inline T2 Dot(Vector<3, T2> const & a, Vector<3, T3> const & b);

    template <typename T2, typename T3>
    friend inline Vector<3, T2> Cross(Vector<3, T2> const & a, Vector<3, T3> const & b);

    template <typename T2, typename T3>
    friend inline Vector<3, T2> Reflect(Vector<3, T2> const & v, Vector<3, T3> const & n);

    template <typename T2, typename T3>
    friend inline Vector<3, T2> Refract(Vector<3, T2> const & v, Vector<3, T3> const & n, double etaiOverEtat);
};

}//namespace rcl

#include "vector3.inl"

#endif