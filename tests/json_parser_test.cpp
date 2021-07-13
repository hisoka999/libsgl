#include "gtest/gtest.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <fstream>
#ifdef __linux
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
TEST(ParserTest, ParseArray)
{

    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto vec = parser->parseArray("[1,2,3,4,\"DATA\"]");
    EXPECT_EQ(std::get<int>(vec.at(0)), 1);
    EXPECT_EQ(std::get<int>(vec.at(1)), 2);
    EXPECT_EQ(std::get<int>(vec.at(2)), 3);
    EXPECT_EQ(std::get<int>(vec.at(3)), 4);
    EXPECT_EQ(std::get<std::string>(vec.at(4)), "DATA");
}

TEST(ParserTest, ParseObject)
{
    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto obj = parser->parseObject("\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22},\"alive\":false  ");

    auto attr = obj->getObjectValue("attr");

    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");
    EXPECT_EQ(attr->getIntValue("test"), 22);
    EXPECT_EQ(obj->getIntValue("alive"), 0);
}

TEST(ParserTest, ParseArrayWithObject)
{

    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto vec = parser->parseArray("[1,{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22},\n},3,4,\"DATA\"]");
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(std::get<int>(vec.at(0)), 1);

    EXPECT_EQ(std::get<int>(vec.at(2)), 3);
    EXPECT_EQ(std::get<int>(vec.at(3)), 4);
    EXPECT_EQ(std::get<std::string>(vec.at(4)), "DATA");

    auto obj = std::get<std::shared_ptr<utils::JSON::Object>>(vec.at(1));

    auto attr = obj->getObjectValue("attr");

    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");
    EXPECT_EQ(attr->getIntValue("test"), 22);
}

TEST(ParserTest, ParseArrayMultipleObjects)
{
    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto vec = parser->parseArray("[{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22,\"t\":3,},\"x\":22 \n},{\"id\":2 ,\"name\":\"Demo\", \"attr\" : {\"test\":22,},\n}]");
    EXPECT_EQ(vec.size(), 2);
    auto obj = std::get<std::shared_ptr<utils::JSON::Object>>(vec.at(0));
    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");

    auto obj2 = std::get<std::shared_ptr<utils::JSON::Object>>(vec.at(1));
    EXPECT_EQ(obj2->getIntValue("id"), 2);
    EXPECT_EQ(obj2->getStringValue("name"), "Demo");
}

TEST(ParserTest, ParseObjectWithArray)
{
    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();
    auto obj = parser->parseObject("\"id\":1 ,\"name\":\"Test\", \"texture\": [\n\"path1\",\"path2\",\"file.ext\"\n]  ");
    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");

    auto vec = obj->getArray("texture");
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(std::get<std::string>(vec.at(0)), "path1");
    EXPECT_EQ(std::get<std::string>(vec.at(1)), "path2");
    EXPECT_EQ(std::get<std::string>(vec.at(2)), "file.ext");
}

TEST(ParserTest, LoadComplexSaveGame)
{
    auto prog_dir = std::string(get_current_dir_name());
    std::string saveGameFile = "/home/stefan/projects/libsgl/tests/test_files/savegame.save";
    std::ifstream file;
    std::istringstream is;
    std::string s;
    std::string group;
    //  std::cout << filename << std::endl;

    file.open(saveGameFile.c_str());
    EXPECT_TRUE(file.is_open());

    std::string buffer((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    utils::JSON::Parser parser;
    auto jsonObject = parser.parseObject(buffer);

    file.close();
    //for debugging
    for (auto name : jsonObject->getAttributes())
    {
        std::cout << "found attr: " << name << std::endl;
    }
    EXPECT_EQ(jsonObject->getAttributes().size(), 5);
}