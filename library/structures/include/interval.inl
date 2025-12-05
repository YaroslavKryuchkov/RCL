#include "constants.hpp"

namespace rcl
{

template <typename T>
rcl::Interval<T>::Interval() : min(-rcl::infinity), max(+rcl::infinity) {}
template <typename T>
template <typename T2, typename T3>
rcl::Interval<T>::Interval(const T2& min, const T3& max) : min((T)min), max((T)max) {}
template <typename T>
rcl::Interval<T>::Interval(Interval<T> a, Interval<T> b)
{
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

template <typename T>
T rcl::Interval<T>::Size() const
{
    return max - min;
}

template <typename T>
template <typename T2>
bool rcl::Interval<T>::Contains(T2 value) const
{
    return min <= value && value <= max;
}

template <typename T>
template <typename T2>
bool rcl::Interval<T>::Hold(T2 value) const
{
    return min < value && value < max;
}

template <typename T>
template <typename T2>
T rcl::Interval<T>::Clamp(T2 value) const
{
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

template <typename T>
Interval<T> rcl::Interval<T>::Expanded(T delta) const
{
    T padding = delta / 2;
    return rcl::Interval<T>(min - padding, max + padding);
}

template <typename T>
const rcl::Interval<double> rcl::Interval<T>::empty = rcl::Interval<double>(+rcl::infinity, -rcl::infinity);

template <typename T>
const rcl::Interval<double> rcl::Interval<T>::universal = rcl::Interval<double>(-rcl::infinity, +rcl::infinity);

}