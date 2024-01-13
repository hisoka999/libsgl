#include "localisation.h"
#include <algorithm>
#include <cstdlib>
#include <engine/utils/exceptions.h>
#include <engine/utils/string.h>
#include <fstream>
#include <magic_enum.hpp>
#include <sstream>
#include <exception>
#include "engine/utils/logger.h"
#include <cmath>

#ifdef _WIN32
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#endif

#ifdef _WIN32

/**
 * @brief Find DLL entry in lang_map for language for specified locale
 */
static WORD GetLangFromLocale(LCID lcid)
{
    TCHAR buff[8] = {0};
    WORD langID = 0;
    if (GetLocaleInfo(lcid, LOCALE_IDEFAULTLANGUAGE, buff, static_cast<int>(std::size(buff))))
        _stscanf_s(buff, _T("%4hx"), &langID);
    return langID;
}
#endif // _WIN32
Localisation::Localisation()
{

    currencyMap[Language::de] = {"Euro", "EUR", "€", ",", "."};
    currencyMap[Language::en] = {"Pound", "GBP", "£", ".", ","};
}

std::string format_currency(double amount)
{
    Currency &currency = Localisation::Instance().getCurrency();

    long base = amount;
    double remainder = std::fmod(std::abs(amount), 1);

    std::string remainderString = std::to_string(long(remainder * 100));
    std::string baseString = std::to_string(std::abs(base));
    std::string result = "";
    int offset = 0;
    int pos = 1;

    std::stringstream stream;

    if (base < 0)
    {
        stream << "-";
    }

    if (std::abs(base) > 999)
    {
        for (int i = baseString.size(); i >= 0; --i)
        {
            if (pos % 3 == 0)
            {
                // TODO baseString = baseString.substr(i, 3);
                if (i - 1 > 0)
                {
                    result = currency.thousandsSeparator + baseString.substr(i - 1, 3) + result;
                }
                else
                {
                    result = baseString.substr(i - 1, 3) + result;
                }

                offset++;
            }
            if (pos > (int(baseString.size()) / 3 * 3))
            {
                std::string remainderX = baseString.substr(0, i);
                result = remainderX + result;
                break;
            }
            pos++;
        }
        stream << result;
    }
    else
        stream << base;

    stream << currency.decimalSeparator << remainderString << " " << currency.unicodeValue;
    return stream.str();
}

Language Localisation::getLang() const
{
    return lang;
}

Currency &Localisation::getCurrency()
{
    return currencyMap.at(lang);
}

std::string Localisation::getLanguage() const
{
    return language;
}

std::locale Localisation::getLocale()
{
#ifdef __linux
    switch (lang)
    {
    case Language::de:
        return std::locale("de_DE.utf8");
    case Language::en:
        return std::locale("en_US.utf8");
    default:
        return std::locale("en_US.utf8");
    }
#elif _WIN32
    // TODO fix later
    switch (lang)
    {
    case Language::de:
        return std::locale("de-DE.utf8");
    case Language::en:
        return std::locale("en-US.utf8");
    default:
        return std::locale("en-US.utf8");
    }
#else
    throw std::runtime_error("unsupported operating system");
#endif
}

void Localisation::loadLanguage(const Language lang, const std::string &appName)
{
    this->lang = lang;

    language = std::string(magic_enum::enum_name(lang));
    std::string fileName = "locale/" + language + "/LC_MESSAGES/" + appName + ".po";
    loadLocalisation(fileName);
}
void Localisation::detectLanguage(const std::string &appName)
{
    std::string lang = "en";
#ifdef __linux
    const char *envLang = std::getenv("LANG");
    language = std::string(envLang);
#elif _WIN32
    WORD Lang1 = GetLangFromLocale(GetThreadLocale());

    WORD Lang2 = GetLangFromLocale(LOCALE_USER_DEFAULT);
    WORD Lang3 = GetLangFromLocale(LOCALE_SYSTEM_DEFAULT);
    if (Lang1 == 0)
    {
        Lang1 = Lang2;
    }
    std::string s;
    if (int cch = GetLocaleInfo(Lang1, LOCALE_SNAME, 0, 0))
    {
        s.resize(cch - 1);
        GetLocaleInfo(Lang1, LOCALE_SNAME, &*s.begin(), cch);
    }
    language = s;
#endif

    if (language.empty())
    {
        language = lang;
    }
    SGL_LOG_TRACE("language = " + language);
    language = language.substr(0, 2);
    SGL_LOG_TRACE("language = " + language);
    std::transform(language.begin(), language.end(), language.begin(), ::tolower);
    this->lang = magic_enum::enum_cast<Language>(language).value();

    loadLanguage(this->lang, appName);
}

std::string Localisation::translate(const std::string &msgid) const
{
    if (translations.count(msgid) > 0)
    {
        return translations.at(msgid);
    }
    return msgid;
}

void Localisation::loadLocalisation(std::string filename)
{
    std::fstream file;
    std::istringstream is;
    std::string s;
    std::string group;
    SGL_LOG_INFO("load locale " + filename);

    file.open(filename.c_str(), std::ios::in);
    if (!file.is_open())
    {
        throw IOException(filename, "file does not exists");
    }
    std::string msgid = "";
    std::string msgstr = "";
    while (getline(file, s))
    {

        std::size_t pos = 0;
        pos = s.find("#");
        if (pos < s.length() - 1)
        {
        }
        else
        {
            pos = s.find("msgid");
            if (pos < s.length() - 1)
            {
                std::string part = s.substr(pos + 5);
                msgid = utils::trim(utils::trim(part), "\"");
            }
            else
            {
                pos = s.find("msgstr");
                if (pos < s.length() - 1)
                {
                    std::string part = s.substr(pos + 6);
                    msgstr = utils::trim(utils::trim(part), "\"");
                    translations[msgid] = msgstr;
                }
            }
        }
    }
    // while(!s.empty());
    file.close();
}
