#include "JSONNode.h"
#include "Tokenizer.h"
#include <memory>

class JSONParser
{
    std::fstream file;
    std::shared_ptr<JSONNode> root;
    std::unique_ptr<JSONNode> current;
    Tokenizer tokenizer;

public:
    JSONParser(const std::string filename) : tokenizer(filename) {}

    void parse()
    {
        std::string key = "";
        while (tokenizer.hasMoreTokens())
        {
            Token token;
            try
            {
                token = tokenizer.getToken();
                std::cout << token.toString() << std::endl;
                switch (token.type)
                {
                case TOKEN::CURLY_OPEN:
                {
                    std::shared_ptr<JSONNode> parsedObject = parseObject();
                    parsedObject->printNode(0);
                    if (!root)
                    {
                        root = parsedObject;
                    }
                    break;
                }
                case TOKEN::ARRAY_OPEN:
                {
                    std::shared_ptr<JSONNode> parsedList = parseList();
                    parsedList->printNode(0);
                    if (!root)
                    {
                        root = parsedList;
                    }
                    break;
                }

                case TOKEN::STRING:
                {
                    tokenizer.rollBackToken();
                    std::shared_ptr<JSONNode> parsedString = parseString();
                    parsedString->printNode(0);
                    if (!root)
                    {
                        root = parsedString;
                    }
                    break;
                }
                case TOKEN::NUMBER:{
                    tokenizer.rollBackToken();
                    std::shared_ptr<JSONNode> parsedNumber = parseNumber();
                    parsedNumber->printNode(0);
                    if(!root){
                        root = parsedNumber;
                    }
                    break;
                }
                }
            }

            catch (std::logic_error e)
            {
                break;
            }
        }
        //assert token not valid
    }

    std::shared_ptr<JSONNode> parseObject()
    {
        std::cout << "Parsing object " << std::endl;
        std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
        std::map<std::string, std::shared_ptr<JSONNode>> keyObjectMap;
        bool hasCompleted = false;
        while (!hasCompleted)
        {
            if (tokenizer.hasMoreTokens())
            {
                Token nextToken = tokenizer.getToken();
                std::string key = nextToken.value;
                std::cout << key << std::endl;
                tokenizer.getToken();
                nextToken = tokenizer.getToken();
                std::shared_ptr<JSONNode> node;
                switch (nextToken.type)
                {
                case TOKEN::STRING:
                {
                    tokenizer.rollBackToken();
                    keyObjectMap[key] = parseString();
                    break;
                }
                case TOKEN::ARRAY_OPEN:
                {
                    keyObjectMap[key] = parseList();
                    break;
                }
                case TOKEN::NUMBER:
                {
                    tokenizer.rollBackToken();
                    keyObjectMap[key] = parseNumber();
                    break;
                }
                case TOKEN::CURLY_OPEN:
                {
                    keyObjectMap[key] = parseObject();
                    break;
                }
                }
                nextToken = tokenizer.getToken();
                if (nextToken.type == TOKEN::CURLY_CLOSE)
                {
                    hasCompleted = true;
                    break;
                }
            }
            else
            {
                throw std::logic_error("No more tokens");
            }
        }
        node->setObject(keyObjectMap);
        return node;
    }
    std::shared_ptr<JSONNode> parseString()
    {
        std::cout << "Parsing String" << std::endl;
        std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
        Token token = tokenizer.getToken();
        node->setString(token.value);
        return node;
    }
    std::shared_ptr<JSONNode> parseNumber()
    {
        std::cout << "Parsing Number" << std::endl;
        std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
        Token nextToken = tokenizer.getToken();
        std::string value = nextToken.value;
        std::cout << value << std::endl;
        float fValue = std::stof(value);
        node->setNumber(fValue);
        return node;
    }

    std::shared_ptr<JSONNode> parseList()
    {
        std::cout << "Parsing List" << std::endl;
        std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
        std::vector<std::shared_ptr<JSONNode>> list;
        bool hasCompleted = false;
        while (!hasCompleted)
        {
            if (!tokenizer.hasMoreTokens())
            {
                throw std::logic_error("No more tokens");
            }
            else
            {
                Token nextToken = tokenizer.getToken();
                std::shared_ptr<JSONNode> node;
                switch (nextToken.type)
                {
                case TOKEN::ARRAY_OPEN:
                {
                    node = parseList();
                    break;
                }
                case TOKEN::CURLY_OPEN:
                {
                    node = parseObject();
                    break;
                }
                case TOKEN::STRING:
                {
                    tokenizer.rollBackToken();
                    node = parseString();
                    break;
                }
                case TOKEN::NUMBER:
                {
                    tokenizer.rollBackToken();
                    node = parseNumber();
                    break;
                }
                }
                list.push_back(node);
                nextToken = tokenizer.getToken();
                if (nextToken.type == TOKEN::ARRAY_CLOSE)
                {
                    hasCompleted = true;
                }
            }
        }
        node->setList(list);
        return node;
    }
};
