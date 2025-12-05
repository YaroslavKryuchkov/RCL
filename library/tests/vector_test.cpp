#include <iostream>

#include "vector.hpp"

int main()
{
    rcl::vec3 A(2, 2, 2);
    rcl::vec3 B(1, 5, 10);
    float c = 4;

    std::cout << "A+B=(" << A+B << ")" << std::endl;
    std::cout << "A-B=(" << A-B << ")" << std::endl;
    std::cout << "A*B=(" << A*B << ")" << std::endl;
    std::cout << "A/B=(" << A/B << ")" << std::endl;
    std::cout << "Dot(A,B)=(" << rcl::Dot(A,B) << ")" << std::endl;
    std::cout << "B+c=(" << B+c << ")" << std::endl;
    std::cout << "B-c=(" << B-c << ")" << std::endl;
    std::cout << "B*c=(" << B*c << ")" << std::endl;
    std::cout << "B/c=(" << B/c << ")" << std::endl;

    int F;
    std::cin >> F;
}