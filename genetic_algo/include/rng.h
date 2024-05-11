#ifndef RNG_HEADER
#define RNG_HEADER

#include <random>

namespace dl
{

static std::random_device _random_device;
static std::mt19937_64 random_engine{_random_device()};

}

#endif // #define RNG_HEADER
