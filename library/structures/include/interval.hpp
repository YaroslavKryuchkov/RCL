#ifndef RCL_INTERVAL
#define RCL_INTERVAL

namespace rcl
{

template <typename T>
struct Interval
{
    T min;
    T max;

    Interval();
    template <typename T2, typename T3>
    Interval(const T2& min, const T3& max);
    Interval(Interval<T> a, Interval<T> b);
    
    T Size() const;

    template <typename T2>
    bool Contains(T2 value) const;
    template <typename T2>
    bool Hold(T2 value) const;
    template <typename T2>
    T Clamp(T2 value) const;
    Interval<T> Expanded(T delta) const;

    static const rcl::Interval<double> empty;
    static const rcl::Interval<double> universal;
};

}

#include "interval.inl"

#endif