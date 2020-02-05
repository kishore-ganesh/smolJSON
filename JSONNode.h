#include <map>
#include <vector>
#include <memory>
enum class Type
{
    OBJECT,
    LIST,
    STRING,
    NUMBER
}; // make it private
class JSONNode
{

    std::map<std::string, std::shared_ptr<JSONNode>> object;
    std::vector<std::shared_ptr<JSONNode>> list;
    std::string s;
    float value;
    Type type;

public:
    JSONNode()
    {
    }
    JSONNode(Type t)
    {
        type = t;
    }
    auto returnObject()
    {
        if (type == Type::OBJECT)
        {
            return object;
        }
        throw std::logic_error("Improper return");
    }
    auto returnList()
    {
        if (type == Type::LIST)
        {
            return list;
        }
        throw std::logic_error("Improper return");
    }
    auto returnString()
    {
        if (type == Type::STRING)
        {
            return s;
        }
        throw std::logic_error("Improper return");
    }
    auto returnValue()
    {
        if (type == Type::NUMBER)
        {
            return value;
        }
        throw std::logic_error("Improper return");
    }

    void setObject(std::map<std::string, std::shared_ptr<JSONNode>> object)
    {
        this->object = object;
        type = Type::OBJECT;
        // if(type==Type::OBJECT){
        //     this->object = toAdd;
        // }
    }

    void setString(std::string s)
    {
        this->s = s;
        type = Type::STRING;
    }
    void setNumber(float f)
    {
        this->value = f;
        type = Type::NUMBER;
    }
    void setList(std::vector<std::shared_ptr<JSONNode>> list)
    {
        this->list = list;
        type = Type::LIST;
    }

    std::string toString(int indentationLevel)
    {
        std::string spaceString = std::string(indentationLevel, ' ');
        std::string outputString = "";

        switch (type)
        {
        case Type::STRING:
        {
            outputString += spaceString + s;
            break;
        }
        case Type::NUMBER:
        {
            outputString += spaceString + std::to_string(value);
            break;
        }
        case Type::LIST:
        {
            std::cout << "[";
            int index = 0;
            for (auto node : list)
            {
                outputString += node->toString(indentationLevel + 1);
                if (index < list.size() - 1)
                {
                    outputString += spaceString + ", ";
                }
                index++;
            }
            outputString += spaceString + "]\n";
            break;
        }
        case Type::OBJECT:
        {
            outputString += "{\n";
            for (std::map<std::string, std::shared_ptr<JSONNode>>::iterator i = object.begin(); i != object.end(); i++)
            {
                outputString += spaceString + i->first + ": ";
                outputString += i->second->toString(indentationLevel + 1);
                std::map<std::string, std::shared_ptr<JSONNode>>::iterator next = i;
                next++;
                if ((next) != object.end())
                {
                    outputString += spaceString + ",";
                }
                outputString += spaceString + "\n";
            }
            outputString += "}\n";
            return outputString;
        }
        }
    }

    void printNode(int indentationLevel)
    {
        std::cout << toString(indentationLevel);
    }
};