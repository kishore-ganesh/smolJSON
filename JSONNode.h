#include<map>
#include<vector>
#include<memory>
enum class Type{
    OBJECT,
    LIST,
    STRING,
    NUMBER
}; // make it private
class JSONNode{
    
    std::map<std::string, std::shared_ptr<JSONNode>> object;
    std::vector<std::shared_ptr<JSONNode>> list;
    std::string s;
    float value;
    Type type;
    public:

    JSONNode(){

    }
    JSONNode(Type t){
        type = t;
    }
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

    void setObject(std::map<std::string, std::shared_ptr<JSONNode>> object){
        this->object = object;
        type = Type::OBJECT;
        // if(type==Type::OBJECT){
        //     this->object = toAdd;
        // }
    }

    void setString(std::string s){
        this->s = s;
        type = Type::STRING;
    }

    void setList(std::vector<std::shared_ptr<JSONNode>> list){
        this->list = list;
        type = Type::LIST;
    }

    void printNode(){
        switch(type){
            case Type::STRING:{
                std::cout << "STRING: " << s;
                break;
            }
            case Type::NUMBER:{
                std::cout << "NUM: " << value;
                break;
            }
            case Type::LIST:{
                std::cout << "[";
                for(auto node:list){
                    node->printNode();
                    std::cout << ", ";
                }
                std::cout <<"]" << std::endl;
                break;
            }
            case Type::OBJECT:{
                std::cout << "OBJECT: {";
                for(std::map<std::string, std::shared_ptr<JSONNode>>::iterator i = object.begin(); i!=object.end(); i++){
                    std::cout << i->first << ": ";
                    i->second->printNode();
                    std::cout << "," << std::endl;
                }
                std::cout << "}" << std::endl;
            }
        }
    }
};