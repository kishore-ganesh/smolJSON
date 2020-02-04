#include "JSONNode.h"
#include "Tokenizer.h"
#include <memory>
#define TRANSITION(TOKEN, STATE, NEXT_STATE) transitions[make_pair(Token::TOKEN, State::STATE)] = NEXT_STATE

enum class State
{
    INSIDE_GLOBAL,
    INSIDE_OBJECT,
    INSIDE_LIST,
    VALID_OBJECT,
    VALID_ITEM,
    INVALID,
    POP,
    END,
    END_KEY,
    END_VALUE,
};

class JSONParser
{
    std::fstream file;
    std::stack<State> currentStates;
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
                    parsedObject->printNode();
                    if (!root)
                    {
                        root = parsedObject;
                    }
                    break;
                }
                case TOKEN::ARRAY_OPEN:
                {
                    std::shared_ptr<JSONNode> parsedList = parseList();
                    parsedList->printNode();
                    if (!root)
                    {
                        root = parsedList;
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
