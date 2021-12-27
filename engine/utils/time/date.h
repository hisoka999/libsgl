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
            Date(const uint16_t year, const uint16_t month, const uint16_t day);
            ~Date();
            void addDay(uint16_t value);

            std::string format();

            uint16_t getYear() const;
            uint16_t getMonth() const;
            uint16_t getDay() const;
            uint64_t toNumber() const;

            bool operator==(const Date &date);
            bool operator>=(const Date &date);
            bool operator>(const Date &date);
            bool operator<=(const Date &date);
            bool operator<(const Date &date);
        };

    } // namespace time

} // namespace utils

#endif // __DATE_H__