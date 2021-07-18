#include "parser.h"
#include <algorithm>
#include <iostream>

namespace utils
{
    namespace JSON
    {

        Parser::Parser()
        {
        }

        void Parser::parseString(const std::string &jsonData)
        {

            for (size_t i = 0; i < jsonData.length(); ++i)
            {
                char data = jsonData[i];
                switch (data)
                {
                case '[': //array begin
                    //parse sub string
                    parseArray(jsonData.substr(i + 1));
                    break;
                case ']': //array end
                    break;
                case '{': //object begin
                    parseObject(jsonData.substr(i + 1));
                    break;
                case '}': //object end
                    break;
                case ':': //object end
                    break;
                }
            }
        }

        size_t Parser::findPositionInString(const std::string data, char starChar, char endChar, const size_t startPosition)
        {
            int depth = 0;
            for (size_t pos = startPosition; pos < data.size(); ++pos)
            {
                if (data.at(pos) == starChar)
                {
                    depth++;
                }
                else if (data.at(pos) == endChar)
                {
                    if (depth > 0)
                    {
                        depth--;
                    }
                    else
                    {
                        return pos;
                    }
                }
            }
            return 0;
        }

        JsonArray Parser::parseArray(const std::string &jsonData)
        {
            JsonArray vector;

            size_t startPos = jsonData.find_first_of("[");
            size_t endPos = jsonData.find_last_of("]");
            if (startPos > jsonData.size())
                return vector;

            std::string data = jsonData.substr(startPos + 1, endPos - startPos - 1);
            //std::cout << "data = " << data << std::endl;

            size_t splitPos = data.find_first_of(",");
            size_t lastSplitPos = 0;
            while (lastSplitPos < data.size())
            {

                std::string attrValue = data.substr(lastSplitPos, splitPos - lastSplitPos);

                if (trim(attrValue) == "")
                    break;

                //std::cout << "array value (start): <<" << attrValue << ">>" << std::endl;
                if (attrValue.find_first_of("{") < attrValue.size())
                {
                    size_t objectStart = data.find_first_of("{", lastSplitPos) + 1;
                    splitPos = findPositionInString(data, '{', '}', objectStart + 1);
                    attrValue = data.substr(objectStart, splitPos - objectStart);
                    //attrValue = rtrim(ltrim(ltrim(ltrim(attrValue, "\n"), " "), "{"), "}");
                    // std::cout << "tmp: " << attrValue << std::endl
                    //           << "----------" << std::endl
                    //           << "data: " << data << std::endl;

                    vector.push_back(parseObject(attrValue));
                    splitPos = data.find_first_of(",", splitPos - 2);
                }
                else if (attrValue.find_first_of("\"") < attrValue.size())
                {
                    //found string
                    vector.push_back(trim(attrValue, "\""));
                }
                else if (attrValue == "true")
                {
                    vector.push_back(true);
                }
                else if (attrValue == "false")
                {
                    vector.push_back(false);
                }
                else if (attrValue.find_first_of(".") < attrValue.size())
                {
                    float value = std::atof(attrValue.c_str());
                    vector.push_back(value);
                }
                else
                {
                    vector.push_back(std::atoi(attrValue.c_str()));
                }
                //std::cout << "array value: " << attrValue << std::endl;

                if (splitPos > data.size())
                    break;
                lastSplitPos = splitPos + 1;
                splitPos = data.find_first_of(",", splitPos + 1);
                //std::cout << "split pos: " << splitPos << std::endl;
                //std::cout << "last split pos: " << lastSplitPos << std::endl;
            };
            return vector;
        }

        std::shared_ptr<Object> Parser::parseObject(const std::string &jsonData)
        {
            std::shared_ptr<Object> object = std::make_shared<Object>();
            size_t lastAttr = 0;
            std::string tmp = jsonData;
            std::string _jsonData = trim(tmp, "{}");

            bool hasAttr = true;

            while (hasAttr)
            {
                size_t splitPos = _jsonData.find_first_of(":", lastAttr);
                size_t endPos = _jsonData.find_first_of(",", lastAttr);
                if (splitPos > _jsonData.size())
                    break;
                //std::cout << splitPos << ":" << endPos << std::endl;

                std::string attrName = _jsonData.substr(lastAttr, splitPos - lastAttr);

                attrName = trim(trim(trim(attrName, "\t\n\r\f\v"), "\""), ",");
                auto filter = [](unsigned char ch)
                { return ch != '\"' && !std::isspace(ch); };
                attrName.erase(std::find_if(attrName.rbegin(), attrName.rend(), filter)
                                   .base(),
                               attrName.end());
                attrName.erase(attrName.begin(), std::find_if(attrName.begin(), attrName.end(), filter));
                hasAttr = !attrName.empty();
                if (hasAttr)
                {
                    if (endPos > _jsonData.size())
                        endPos = _jsonData.size();
                    std::string attrValue = _jsonData.substr(splitPos + 1, endPos - splitPos - 1);
                    attrValue = trim(attrValue);
                    attrName = trim(attrName);

                    //std::cout << attrName << ":" << attrValue << std::endl;
                    //test if the value is a string
                    size_t arrayPos = attrValue.find_first_of("[");
                    size_t objectPos = attrValue.find_first_of("{");
                    if (objectPos < attrValue.size() && objectPos < arrayPos)
                    {
                        size_t objectStart = _jsonData.find_first_of("{", splitPos);
                        size_t objectEnd = findPositionInString(_jsonData, '{', '}', objectStart + 1);
                        attrValue = _jsonData.substr(objectStart + 1, objectEnd - objectStart - 1);
                        attrValue = rtrim(ltrim(trim(attrValue), "{"), "}");
                        //std::cout << "obj value:" << attrName << ":" << attrValue << std::endl;
                        if (attrValue.size() == 0)
                        {
                            object->setAttribute(attrName, std::make_shared<Object>());
                        }
                        else
                        {
                            object->setAttribute(attrName, parseObject(attrValue));
                        }

                        endPos = objectEnd + 1;
                    }
                    else if (arrayPos < attrValue.size())
                    {
                        size_t arrayStart = _jsonData.find_first_of("[", splitPos);
                        size_t arrayEnd = findPositionInString(_jsonData, '[', ']', arrayStart + 1);
                        if (arrayEnd == 0)
                            arrayEnd = arrayStart + 1;
                        //splitPos = attrValue.find_first_of("]");
                        attrValue = _jsonData.substr(arrayStart, arrayEnd - arrayStart + 1);
                        //attrValue = rtrim(ltrim(trim(attrValue), "["), "]");
                        //std::cerr << "array value: " << attrValue << std::endl;
                        object->setArrayAttribute(attrName, parseArray(attrValue));
                        endPos = arrayEnd + 1;
                    }
                    else if (attrValue.find_first_of("\"") < attrValue.size())
                    {
                        //found string
                        object->setAttribute(attrName, trim(attrValue, "\""));
                    }

                    else if (attrValue == "true")
                    {
                        object->setAttribute(attrName, true);
                    }
                    else if (attrValue == "false")
                    {
                        object->setAttribute(attrName, false);
                    }
                    else if (attrValue.find_first_of(".") < attrValue.size())
                    {
                        float value = std::atof(attrValue.c_str());
                        object->setAttribute(attrName, value);
                    }
                    else
                    {
                        object->setAttribute(attrName, std::atoi(attrValue.c_str()));
                    }

                    lastAttr = endPos + 1;
                }
            }

            return object;
        }

    }
}
