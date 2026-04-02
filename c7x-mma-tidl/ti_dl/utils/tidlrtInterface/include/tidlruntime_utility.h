
#ifndef TIDLRUNTIME_UTILITY_H
#define TIDLRUNTIME_UTILITY_H

#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <cstring>
#include <regex>

#include "configparser.h"

#define MAX_ITEMS_TO_PARSE (512)

typedef struct {
    // Pair of keys
    std::pair<std::string, std::string>     key;

    // Value to set
    void                                    *data;

    // Datatype of the value
    infoType_t                              datatype;

    // Max Size in case of MULTI category or STRING datatype 
    size_t                                  size;

    // Is data expecting array type string seperated by space?
    valueCategory_t                         category;

    // Does data need translation
    bool                                    translate;

    // Parsed from: -1 : Not parsed, 0 - TIDLRT Key , 1 - OSRT Key
    int32_t                                 parseType;

    // Parse Count
    int32_t                                 parseCount;
} sOptionMapping;

inline std::string trimString(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

inline std::map<std::string, std::string> trimStringMap(const std::map<std::string, std::string> &originalMap)
{
    std::map<std::string, std::string> trimmedMap = {};
    for (const auto& pair : originalMap)
    {
        std::string trimmedKey = trimString(pair.first);
        std::string trimmedValue = trimString(pair.second);
        trimmedMap[trimmedKey] = trimmedValue; 
    }
    return trimmedMap;
}

template <typename T>
inline int32_t parseNumericOption(T *option, const std::string &value)
{
    int32_t count = 0;
    std::stringstream ss(value);
    T val;
    ss >> val;
    if (!ss.fail())
    {
        *option = val;
        count++;
    }
    else
    {
        count = 0;
    }
    return count;
}

inline int32_t parseStringOption(char *option, size_t size, const std::string &value)
{
    int32_t count = 0;
    if(value.size() > size)
    {
        count = 0;
    }
    else
    {
        strcpy(option, value.c_str());
        count++;
    }
    return count;
}

template <typename T>
inline int32_t parseMultiNumericOption(T *option, size_t size, const std::string &value)
{
    int32_t count = 0;
    std::string trimmedValue = trimString(value);
    std::regex pattern(R"([+-]?([0-9]*[.])?[0-9]+)");
    auto words_begin = std::sregex_iterator(trimmedValue.begin(), trimmedValue.end(), pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end && count < (int32_t)size; ++i)
    {
        std::smatch match = *i;
        std::string s = match.str();
        int32_t status = parseNumericOption(&option[count], s);
        if (status != 0)
        {
            count++;
        }
    }

    return count;
}


inline int32_t parseEntry(sOptionMapping &entry, void *data, const std::string &value)
{
    int32_t parsedCount = 0;
    switch (entry.datatype)
    {
        case INT_8:
            if (entry.category == SINGLE)
            {
                parsedCount = parseNumericOption((int8_t *)data, value);
            }
            else
            {
                parsedCount = parseMultiNumericOption((int8_t *)data, entry.size, value);
            }
            break;
        case INT_16:
            if (entry.category == SINGLE)
            {
                parsedCount = parseNumericOption((int16_t *)data, value);
            }
            else
            {
                parsedCount = parseMultiNumericOption((int16_t *)data, entry.size, value);
            }
            break;
        case INT_32:
            if (entry.category == SINGLE)
            {
                parsedCount = parseNumericOption((int32_t *)data, value);
            }
            else
            {
                parsedCount = parseMultiNumericOption((int32_t *)data, entry.size, value);
            }
            break;
        case FLOAT:
            if (entry.category == SINGLE)
            {
                parsedCount = parseNumericOption((float *)data, value);
            }
            else
            {
                parsedCount = parseMultiNumericOption((float *)data, entry.size, value);
            }
            break;
        case STRING:
            parsedCount = parseStringOption((char *)data, entry.size, value);
            break;
        case CPP_STRING:
            parsedCount = parseStringOption((char *)data, entry.size, value);
            break;
        default:
            break;
    }

    return parsedCount;

}

inline std::string getEntryString(const sOptionMapping &entry, void *data)
{
    std::string line = "";
    if (entry.key.first == "" && entry.key.second == "")
    {
        return line;
    }

    if (entry.key.first != "" && entry.key.second != "")
    {
        line = entry.key.first + " / " + entry.key.second;
    }
    else if (entry.key.first != "")
    {
        line = entry.key.first;
    }
    else if (entry.key.second != "")
    {
        line = entry.key.second;
    }

    line += " : ";

    switch (entry.datatype)
    {
        case INT_8:
            if (entry.category == SINGLE)
            {
                line += std::to_string(*(int8_t *)data);
            }
            else
            {
                int32_t count = 1;
                if (entry.parseCount > 0) count = entry.parseCount;
                for (int32_t i = 0; i < count; i++)
                {
                    if (i == 0) line += '[';
                    line += std::to_string(*((int8_t *)data+i));
                    if (i != count - 1) line += ", ";
                    else line += ']';
                }
            }
            break;
        case INT_16:
            if (entry.category == SINGLE)
            {
                line += std::to_string(*(int16_t *)data);
            }
            else
            {
                int32_t count = 1;
                if (entry.parseCount > 0) count = entry.parseCount;
                for (int32_t i = 0; i < count; i++)
                {
                    if (i == 0) line += '[';
                    line += std::to_string(*((int16_t *)data+i));
                    if (i != count - 1) line += ", ";
                    else line += ']';
                }
            }
            break;
        case INT_32:
            if (entry.category == SINGLE)
            {
                line += std::to_string(*(int32_t *)data);
            }
            else
            {
                int32_t count = 1;
                if (entry.parseCount > 0) count = entry.parseCount;
                for (int32_t i = 0; i < count; i++)
                {
                    if (i == 0) line += '[';
                    line += std::to_string(*((int32_t *)data+i));
                    if (i != count - 1) line += ", ";
                    else line += ']';
                }
            }
            break;
        case FLOAT:
            if (entry.category == SINGLE)
            {
                line += std::to_string(*(float *)data);
            }
            else
            {
                int32_t count = 1;
                if (entry.parseCount > 0) count = entry.parseCount;
                for (int32_t i = 0; i < count; i++)
                {
                    if (i == 0) line += '[';
                    line += std::to_string(*((float *)data+i));
                    if (i != count - 1) line += ", ";
                    else line += ']';
                }
            }
            break;
        case STRING:
            line += (char *)data;
            break;
        case CPP_STRING:
            line += (char *)data;
            break;
        default:
            break;
    }
    return line;
}

#endif  /* TIDLRUNTIME_UTILITY_H */