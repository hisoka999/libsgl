#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <map>
#include <memory>
#include <mutex>

enum class Language : long
{
    de,
    en
};

struct Currency
{
    std::string name;
    std::string isoKey;
    std::string unicodeValue;
    std::string decimalSeparator;
    std::string thousandsSeparator;
};

class Localisation
{
public:
    static Localisation &Instance()
    {
        static Localisation instance;
        return instance;
    }

    void loadLanguage(const Language lang, const std::string &appName);
    void detectLanguage(const std::string &appName);

    std::string translate(const std::string &msgid) const;

    std::string getLanguage() const;

    std::locale getLocale();

    Language getLang() const;

    Currency &getCurrency();

private:
    void loadLocalisation(std::string filename);
    Localisation();
    ~Localisation() = default;
    Localisation(const Localisation &) = delete;
    Localisation &operator=(const Localisation &) = delete;

    std::map<std::string, std::string> translations;
    std::string language;
    Language lang;
    std::map<Language, Currency> currencyMap;
};

std::string format_currency(double amount);

#endif // LOCALISATION_H
