#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

namespace utils
{

    class PerlinNoise
    {
        std::vector<int> p;

    public:
        // Initialize with the reference values for the permutation vector
        PerlinNoise();
        // Generate a new permutation vector based on the value of seed
        PerlinNoise(unsigned int seed);
        // Get a noise value, for 2D images z can have any value
        double noise(double x, double y, double z);

    private:
        static double fade(double t);
        static double lerp(double t, double a, double b);
        static double grad(int hash, double x, double y, double z);
    };

} // namespace utils

#endif // PERLINNOISE_H
