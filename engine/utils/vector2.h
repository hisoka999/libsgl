#ifndef UTILS_VECTOR2_H
#define UTILS_VECTOR2_H

namespace utils
{

    bool areSame(const float a, const float b);

    class Vector2
    {
    public:
        /** Default constructor */
        Vector2(const float x, const float y);
        Vector2();
        /** copy constructor */
        Vector2(const Vector2 &pOriginal);
        /** Default destructor */
        virtual ~Vector2();
        float getX() const
        {
            return x;
        }
        float getY() const
        {
            return y;
        }
        float distance(const Vector2 &v2);
        const Vector2 operator+(const Vector2 &other) const;
        const Vector2 operator-(const Vector2 &other) const;
        const Vector2 operator*(const Vector2 &other) const;
        const Vector2 operator*(const float value) const;
        void operator+=(const Vector2 &other);
        void operator-=(const Vector2 &other);
        bool operator==(const Vector2 &other);
        bool operator!=(const Vector2 &other);
        Vector2 &operator=(const Vector2 &other);

        const Vector2 normalize();
        const Vector2 floor();

    protected:
    private:
        float x, y;
    };
    // Vector2 operator+(const Vector2 &orig, const Vector2 &other);
    // Vector2 operator-(const Vector2 &orig, const Vector2 &other);
    // Vector2 operator*(const Vector2 &orig, const Vector2 &other);
    // Vector2 operator*(const Vector2 &orig, float value);
    Vector2 lerp(const Vector2 &start, const Vector2 &end, const float percentage);
} // namespace utils

#endif // UTILS_VECTOR2_H
