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

    using JSONObject = std::map<std::string, std::shared_ptr<JSONNode>>; 
    using JSONList = std::vector<std::shared_ptr<JSONNode>>; 
    union Values{
    JSONObject object;
    JSONList list;
    std::string s;
    float value;
    Values(JSONObject &object){
        this->object = object;
    }
    } values;    
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
            return values.object;
        }
        throw std::logic_error("Improper return");
    }
    auto returnList()
    {
        if (type == Type::LIST)
        {
            return values.list;
        }
        throw std::logic_error("Improper return");
    }
    auto returnString()
    {
        if (type == Type::STRING)
        {
            return values.s;
        }
        throw std::logic_error("Improper return");
    }
    auto returnValue()
    {
        if (type == Type::NUMBER)
        {
            return values.value;
        }
        throw std::logic_error("Improper return");
    }

    void setObject(JSONObject object) {
        new (&values.object) JSONObject; 
        this->values.object = object;
        type = Type::OBJECT;
        // if(type==Type::OBJECT){
        //     this->object = toAdd;
        // }
    }

    void setString(std::string s)
    {
        this->values.s = s;
        type = Type::STRING;
    }
    void setNumber(float f)
    {
        this->values.value = f;
        type = Type::NUMBER;
    }
    void setList(JSONList list)
    {
        new (&values.list) JSONList; 
        this->values.list = list;
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
            outputString += spaceString + values.s;
            break;
        }
        case Type::NUMBER:
        {
            outputString += spaceString + std::to_string(values.value);
            break;
        }
        case Type::LIST:
        {
            std::cout << "[";
            int index = 0;
            for (auto node : values.list)
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
            for (JSONObject::iterator i = values.object.begin(); i != values.object.end(); i++)
            {
                outputString += spaceString + i->first + ": ";
                outputString += i->second->toString(indentationLevel + 1);
                JSONObject::iterator next = i;
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
