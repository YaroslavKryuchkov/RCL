#ifndef RCL_SAMPLER
#define RCL_SAMPLER

#include "vector.hpp"
#include "functions.hpp"

namespace rcl
{

class Sampler
{
public:
    Sampler(int samples)
    {
        sqrtSPP = int(std::sqrt(samples));
        invSqrtSPP = 1.0 / sqrtSPP;
    }

    vec3 GetSampleOffset(int sample) const
    {
        if(sqrtSPP * sqrtSPP > sample)
            return SquareStratified(sample % sqrtSPP, sample / sqrtSPP);
        else
            return RandomSample();
    }

private:
    int sqrtSPP;
    double invSqrtSPP;

    vec3 RandomSample() const
    {
        return vec3(RandomDouble01(), RandomDouble01(), 0);
    } 

    vec3 SquareStratified(int s_i, int s_j) const
    {
        double px = ((s_i + RandomDouble01()) * invSqrtSPP) - 0.5;
        double py = ((s_j + RandomDouble01()) * invSqrtSPP) - 0.5;
        return vec3(px, py, 0);
    }
};

}
#endif