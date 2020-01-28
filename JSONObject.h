#include<map>
#include<vector>
enum class Type{
    OBJECT,
    LIST,
    STRING,
    NUMBER
}; // make it private
class JSONObject{
    JSONObject(Type t){
        type = t;
    }
    std::map<std::string, JSONObject> object;
    std::vector<JSONObject> list;
    std::string s;
    float value;
    Type type;

    auto returnObject(){
        if(type==Type::OBJECT){
            return object;
        }
        throw std::logic_error("Improper return");
    }
    auto returnList(){
        if(type==Type::LIST){
            return list;
        }
        throw std::logic_error("Improper return");
    }
    auto returnString(){
        if(type==Type::STRING){
            return s;
        }
        throw std::logic_error("Improper return");
    }
    auto returnValue(){
        if(type==Type::NUMBER){
            return value;
        }
        throw std::logic_error("Improper return");
    }

    void addObject(JSONObject toAdd){
        if(type==Type::OBJECT){
            this->object 
        }
    }
};