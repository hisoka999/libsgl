#include "date.h"
#include <engine/utils/string.h>
namespace utils
{
    namespace time
    {
        const uint16_t lastDayOfMonth(const uint16_t year, const uint16_t month)
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
        const bool isLeapYear(const uint16_t year)
        {
            return false; //TODO implement it the right way
        }
        Date::Date(uint16_t year, uint16_t month, uint16_t day)
            : year(year), month(month), day(day)
        {
        }

        Date::~Date()
        {
        }

        void Date::addDay(uint16_t value)
        {
            day += value;
            while (lastDayOfMonth(year, month) > day)
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
            return utils::string_format("%2i.%2i.%i", day, month + 1, year);
        }

        uint16_t Date::getYear()
        {
            return year;
        }

        uint16_t Date::getMonth()
        {
            return month;
        }

        uint16_t Date::getDay()
        {
            return day;
        }
    }
}