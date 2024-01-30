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

            [[nodiscard]] uint16_t getYear() const;
            [[nodiscard]] uint16_t getMonth() const;
            [[nodiscard]] uint16_t getDay() const;
            [[nodiscard]] uint64_t toNumber() const;
            [[nodiscard]] uint16_t getDayOfWeek();

            bool operator==(const Date &date) const;
            bool operator>=(const Date &date) const;
            bool operator>(const Date &date) const;
            bool operator<=(const Date &date) const;
            bool operator<(const Date &date) const;
        };

    } // namespace time

} // namespace utils

#endif // __DATE_H__
