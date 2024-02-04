#pragma once
#include <cstdint>
#include "date.h"

namespace utils
{
    namespace time
    {
        class Time
        {
        private:
            Date date;
            uint8_t hours;
            uint8_t minutes;
            uint8_t seconds;

        public:
            Time(const uint16_t year, const uint16_t month, const uint16_t day, const uint8_t hours,
                 const uint8_t minutes, const uint8_t seconds);
            ~Time();

            Date &toDate();
            uint8_t getHours() const;
            uint8_t getMinutes() const;
            uint8_t getSeconds() const;
            std::string format();
            void addMinutes(size_t value);
            void addHours(size_t value);

            bool operator==(const Time &time) const;
            bool operator>=(const Time &time) const;
            bool operator>(const Time &time) const;
            bool operator<=(const Time &time) const;
            bool operator<(const Time &time) const;
        };

    } // namespace time

} // namespace utils
