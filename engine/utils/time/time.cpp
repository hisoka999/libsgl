#include "time.h"
#include "engine/utils/string.h"

namespace utils::time
{
    Time::Time(const uint16_t year, const uint16_t month, const uint16_t day, const uint8_t hours,
               const uint8_t minutes, const uint8_t seconds) :
        date(year, month, day),
        hours(hours), minutes(minutes), seconds(seconds)
    {
    }
    Time::~Time() {}
    std::string Time::format()
    {
        return date.format() + utils::string_format(" %.2i:%.2i:%.2i", hours, minutes, seconds);
        ; // TODO
    }

    uint8_t Time::getHours() const { return hours; }

    uint8_t Time::getMinutes() const { return minutes; }

    uint8_t Time::getSeconds() const { return seconds; }

    Date &Time::toDate() { return date; }

    void Time::addHours(size_t value) { addMinutes(value * 60); }

    void Time::addMinutes(size_t value)
    {
        size_t remainingValue = value;

        size_t currentMinutes = minutes;


        currentMinutes += remainingValue;
        while (currentMinutes > 60)
        {
            hours++;
            currentMinutes -= 60;
            remainingValue -= 60;
            if (hours >= 24)
            {
                date.addDay(1);
                hours -= 24;
            }
        }
        minutes = currentMinutes;
    }

    bool Time::operator<(const Time &time) const
    {
        if (date < time.date)
        {
            return true;
        }
        else if (hours < time.hours)
        {
            return true;
        }
        else if (minutes < time.minutes)
        {
            return true;
        }
        return seconds < time.seconds;
    }

    bool Time::operator<=(const Time &time) const
    {
        if (this->operator==(time))
            return true;
        return this->operator<(time);
    }
    bool Time::operator==(const Time &time) const
    {
        return date == time.date && hours == time.hours && minutes == time.minutes && seconds == time.seconds;
    }

    bool Time::operator>(const Time &time) const
    {
        if (date > time.date)
        {
            return true;
        }
        else if (hours > time.hours)
        {
            return true;
        }
        else if (minutes > time.minutes)
        {
            return true;
        }
        return seconds > time.seconds;
    }

    bool Time::operator>=(const Time &time) const
    {
        if (this->operator==(time))
            return true;

        return this->operator>(time);
    }


} // namespace utils::time
