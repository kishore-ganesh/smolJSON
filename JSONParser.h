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
    JSONParser(const std::string filename) : tokenizer(filename)
    {
        /*
            INSIDE GLOBAL, ": End
            Inside Global, Curly -> INSIDEObject
            Inside Global, Number -> END
            Inside Global, []: INSIDEArray
            Object, first " -> HAS KEY
            HAS KEY -> "": VALID
            HAS KEY -> NUMBER: VALID
            HAS KEY -> {: Object,
            HAS KEY -> [: INSIDE ARRAY
            INSIDE OBJECT -> }: POP
            INSIDE ARRAY -> ] : POP
            VALID_OBJECT -> ": HAS_KEY (necessary for)
            VL -> NEXT: ? 
            ONLY: INSIDE OBJECT -> HAS KEY -> , -> HAS KEY

         */
        // TRANSITION(STRING, INSIDE_GLOBAL, END);
        // TRANSITION(CURLY_L, INSIDE_GLOBAL, INSIDE_OBJECT);
        // TRANSITION(NUMBER, INSIDE_GLOBAL, END);
        // TRANSITION(ARRAY_OPEN, INSIDE_GLOBAL, INSIDE_ARRAY);
        // TRANSITION(STRING, INSIDE_OBJECT, HAS_KEY);
        // TRANSITION(CURLY_R, INSIDE_OBJECT, POP);
        // TRANSITION(COLON, HAS_KEY, END_KEY);
        // TRANSITION(CURLY_L, END_KEY, INSIDE_OBJECT);
        // TRANSITION(ARRAY_OPEN, END_KEY, INSIDE_ARRAY);
        // TRANSITION(STRING, END_KEY, END_VALUE);
        // TRANSITION(NUMBER, END_KEY, END_VALUE);
        // TRANSITION(COMMA, END_VALUE, VALID_OBJECT);
        // TRANSITION(STRING, VALID_OBJECT, HAS_KEY);
        // TRANSITION(STRING,INSIDE_ARRAY, ARRAY_VALUE_START);
        // TRANSITION(NUMBER, INSIDE_ARRAY, ARRAY_VALUE_START);
        // TRANSITION(CURLY, INSIDE_ARRAY, INSIDE_OBJECT);
        // TRANSITION(ARRAY_OPEN, INSIDE_ARRAY, INSIDE_ARRAY);
        // TRANSITION(COMMA,ARRAY_VALUE_START, INSIDE_ARRAY);
        // TRANSITION(ARRAY_CLOSE,ARRAY_VALUE_START, POP);

        /*
            Check right order of pops
         */

        //on final pop, if inside global, then end
    }

    // struct transition(TOKEN token){
    //     //ON transition
    //     State nextState = transitionFunction(token);
    //     vector<State> popStates = {State::END_KEY, State::END_VALUE,State::HAS_KEY, State::ARRAY_VALUE_START};
    //     if(nextState==State::INVALID){
    //         throw std::exception("Invalid JSON");
    //     }
    //     if(std::any_of(popStates.begin(), popStates.end(), [](State a){return a==currentStates.top()}){
    //         currentStates.pop();
    //     }
    //     if(nextState==State::POP){
    //         currentStates.pop();
    //         if(currentStates.top()==State::INSIDE_ARRAY){
    //             currentStates.push(State::ARRAY_VALUE_START) //rename
    //             // look at the transition
    //         }
    //         if(currentStates.top()==State::INSIDE_OBJECT){
    //             currentStates.push(State::END_VALUE) //When empty object?
    //         }
    //         if(currentStates.top()==State::INSIDE_GLOBAL){
    //             currentStates.push(State::END);
    //         }
    //     } else{
    //         currentStates.push(nextState);

    //     }

    //     //THOSE TOKENS in between need to be popped

    // }

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
                switch(token.type){
                    case TOKEN::CURLY_OPEN: {
                        std::shared_ptr<JSONNode> parsedObject = parseObject();
                        parsedObject->printNode();
                        if(!root){
                            root = parsedObject;
                        }
                        break;
                    }
                    case TOKEN::ARRAY_OPEN:{
                        std::shared_ptr<JSONNode> parsedList = parseList();
                        parsedList->printNode();
                        if(!root){
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
