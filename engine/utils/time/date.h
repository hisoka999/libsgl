#ifndef __DATE_H__
#define __DATE_H__

#include <cstdint>
#include <string>

namespace utils
{
    namespace time
    {
        uint16_t lastDayOfMonth(const uint16_t year, const uint16_t month);
        bool isLeapYear(const uint16_t year);

        class Date
        {
        private:
            uint16_t year;
            uint16_t month;
            uint16_t day;

        public:
            Date(uint16_t year, uint16_t month, uint16_t day);
            ~Date();
            void addDay(uint16_t value);

            std::string format();

            uint16_t getYear();
            uint16_t getMonth();
            uint16_t getDay();
            uint64_t toNumber();

            bool operator==(Date &date);
            bool operator>=(Date &date);
            bool operator>(Date &date);
            bool operator<=(Date &date);
            bool operator<(Date &date);
        };

    } // namespace time

} // namespace utils

#endif // __DATE_H__