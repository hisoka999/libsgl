#include "temperature.h"

namespace utils::temperature
{
    constexpr float convert_c2f(const float celsius) { return (celsius * 1.8) + 32.0; }

    constexpr float convert_f2c(const float fahrenheit) { return (fahrenheit - 32.0) * (5.0 / 9.0); }
} // namespace utils::temperature
