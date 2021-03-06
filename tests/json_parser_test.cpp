#include "gtest/gtest.h"
#include <engine/utils/json/parser.h>
#include <engine/utils/json/Lexer.h>
#include <memory>
#include <fstream>
#ifdef __linux
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <chrono>
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

TEST(LexerTest, LexObject)
{
    utils::JSON::Lexer lexer;
    auto tokens = lexer.lex("{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22},\"alive\":false  }");

    // std::vector<std::string> tokens(tokenList.begin(), tokenList.end());

    EXPECT_EQ(tokens.size(), 21);
    EXPECT_EQ(tokens[0], "{");
    EXPECT_EQ(tokens[1], "id");
    EXPECT_EQ(tokens[2], ":");
    EXPECT_EQ(tokens[3], "1");
    EXPECT_EQ(tokens[4], ",");
    EXPECT_EQ(tokens[5], "name");
    EXPECT_EQ(tokens[6], ":");
    EXPECT_EQ(tokens[7], "Test");
    EXPECT_EQ(tokens[8], ",");
    EXPECT_EQ(tokens[9], "attr");
    EXPECT_EQ(tokens[10], ":");
    EXPECT_EQ(tokens[11], "{");
    EXPECT_EQ(tokens[12], "test");
    EXPECT_EQ(tokens[13], ":");
    EXPECT_EQ(tokens[14], "22");
    EXPECT_EQ(tokens[15], "}");
    EXPECT_EQ(tokens[16], ",");
    EXPECT_EQ(tokens[17], "alive");
    EXPECT_EQ(tokens[18], ":");
    EXPECT_EQ(tokens[19], "false");
    EXPECT_EQ(tokens[20], "}");
}

TEST(ParserTest, ParseObject)
{
    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto obj = parser->parseObject("{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22},\"alive\":false  }");
    EXPECT_TRUE(obj->hasAttribute("attr"));
    EXPECT_TRUE(obj->hasAttribute("name"));
    EXPECT_TRUE(obj->hasAttribute("id"));
    EXPECT_TRUE(obj->hasAttribute("alive"));

    for (auto t : obj->getAttributes())
    {
        std::cout << "attribute: " << t << std::endl;
    }

    auto attr = obj->getObjectValue("attr");
    for (auto t : attr->getAttributes())
    {
        std::cout << "attr attribute: " << t << std::endl;
    }
    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");
    try
    {

        EXPECT_EQ(attr->getIntValue("test"), 22);
        EXPECT_EQ(obj->getBoolValue("alive"), false);
    }
    catch (std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST(ParserTest, ParseArrayWithObject)
{

    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto vec = parser->parseArray("[1,{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22}\n},3,4,\"DATA\"]");
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

    auto vec = parser->parseArray("[{\"id\":1 ,\"name\":\"Test\", \"attr\" : {\"test\":22,\"t\":3},\"x\":22 \n},{\"id\":2 ,\"name\":\"Demo\", \"attr\" : {\"test\":22}\n}]");
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
    auto obj = parser->parseObject("{\"id\":1 ,\"name\":\"Test\", \"texture\": [\n\"path1\",\"path2\",\"file.ext\"\n]  }");
    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getStringValue("name"), "Test");

    auto vec = obj->getArray("texture");
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(std::get<std::string>(vec.at(0)), "path1");
    EXPECT_EQ(std::get<std::string>(vec.at(1)), "path2");
    EXPECT_EQ(std::get<std::string>(vec.at(2)), "file.ext");
}

TEST(ParserTest, ParseNegativeNumbers)
{
    std::shared_ptr<utils::JSON::Parser> parser = std::make_shared<utils::JSON::Parser>();

    auto obj = parser->parseObject("{\"id\":1 ,\"cash\": -2003.45, \"pos\": -332 }");
    EXPECT_TRUE(obj->hasAttribute("id"));
    EXPECT_TRUE(obj->hasAttribute("cash"));
    EXPECT_TRUE(obj->hasAttribute("pos"));
    EXPECT_EQ(obj->getIntValue("id"), 1);
    EXPECT_EQ(obj->getFloatValue("cash"), -2003.45f);
    EXPECT_EQ(obj->getIntValue("pos"), -332);
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
    // for debugging
    for (auto name : jsonObject->getAttributes())
    {
        std::cout << "found attr: " << name << std::endl;
    }
    EXPECT_EQ(jsonObject->getAttributes().size(), 5);
}

TEST(ParserTest, TestSpeed)
{
    auto prog_dir = std::string(get_current_dir_name());
    std::string saveGameFile = "/home/stefan/projects/libsgl/tests/test_files/savegame2.save";
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
    auto start = std::chrono::high_resolution_clock::now();

    auto jsonObject = parser.parseObject(buffer);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "TestSpeed: " << milliseconds << "ms" << std::endl;
    EXPECT_LE(milliseconds, 100);
    // test lexer speed
    start = std::chrono::high_resolution_clock::now();

    utils::JSON::Lexer lexer;
    auto result = lexer.lex(buffer);
    elapsed = std::chrono::high_resolution_clock::now() - start;

    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    EXPECT_LE(milliseconds, 40);
    std::cout << "TestSpeed Lexer: " << milliseconds << "ms for a list of size" << result.size() << std::endl;

    file.close();
    // for debugging
    for (auto name : jsonObject->getAttributes())
    {
        std::cout << "found attr: " << name << std::endl;
    }
    EXPECT_EQ(jsonObject->getAttributes().size(), 6);
}