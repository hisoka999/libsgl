#include "date.h"
#include <engine/utils/string.h>
#include <cmath>

namespace utils
{
    namespace time
    {
        uint16_t lastDayOfMonth(const uint16_t year, const uint16_t month)
        {
            if (month == 1)
            {
                if (isLeapYear(year))
                    return 29;
                return 28;
            }
            else if (month % 2)
            {
                return 30;
            }
            else
            {
                return 31;
            }
        }
        bool isLeapYear(const uint16_t year)
        {
            return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
        }
        Date::Date(const uint16_t year, const uint16_t month, const uint16_t day)
            : year(year), month(month), day(day)
        {
        }

        Date::~Date()
        {
        }

        void Date::addDay(uint16_t value)
        {
            day += value;
            while (lastDayOfMonth(year, month) < day)
            {
                day -= lastDayOfMonth(year, month);
                month++;
                if (month > 11)
                {
                    month = 0;
                    year++;
                }
            }
        }

        std::string Date::format()
        {
            return utils::string_format("%.2i.%.2i.%i", day, month + 1, year);
        }

        uint16_t Date::getYear() const
        {
            return year;
        }

        uint16_t Date::getMonth() const
        {
            return month;
        }

        uint16_t Date::getDay() const
        {
            return day;
        }

        uint64_t Date::toNumber() const
        {
            return (year * 365) + (month * 12) + day;
        }

        uint16_t Date::getDayOfWeek()
        {
            int letter = int(1 + year + std::floor(float(year / 4)) + std::floor(float((year - 1600) / 400)) - std::floor(float((year - 1600) / 100))) % 7;
            std::string weekday = "";

            int dayOfYear = 0;
            for (int month = 0; month <= this->month; month++)
            {
                if (month < this->month)
                    dayOfYear += utils::time::lastDayOfMonth(year, this->month);
                else
                    dayOfYear += this->day;
            }
            int weekOfYear = dayOfYear / 7;
            return (letter + dayOfYear) - (weekOfYear * 7) - 1;
        }

        bool Date::operator==(const Date &date)
        {
            return year == date.getYear() && month == date.getMonth() && day == date.getDay();
        }

        bool Date::operator>=(const Date &date)
        {
            return year >= date.getYear() && month >= date.getMonth() && day >= date.getDay();
        }

        bool Date::operator>(const Date &date)
        {

            return toNumber() > date.toNumber();
        }

        bool Date::operator<=(const Date &date)
        {
            return year <= date.getYear() && month <= date.getMonth() && day <= date.getDay();
        }

        bool Date::operator<(const Date &date)
        {
            return toNumber() < date.toNumber();
        }
    }
}