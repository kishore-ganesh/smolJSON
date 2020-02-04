#include<fstream>
#include<vector>
#include<map>
#include<stdexcept>
#include<stack>
#include<iostream>
enum class TOKEN{
    CURLY_OPEN,
    CURLY_CLOSE,
    COLON,
    STRING,
    NUMBER,
    ARRAY_OPEN,
    ARRAY_CLOSE,
    COMMA
};


struct Token{
    std::string value;
    TOKEN type;
    std::string toString(){
        switch(type){
            case TOKEN::CURLY_OPEN: {
                return "Curly open"; 
            }
            case TOKEN::CURLY_CLOSE:{
                return "Curly close";
            }
            case TOKEN::COLON:{
                return "COLON";
            }
            case TOKEN::NUMBER:{
                return "Number: " + value;
            }
            case TOKEN::STRING:{
                return "String: " + value;
            }

            case TOKEN::ARRAY_OPEN:{
                return "Array open";
            }
            case TOKEN::ARRAY_CLOSE:{
                return "Array close";
            }
            case TOKEN::COMMA:{
                return "Comma";
            }


        }
    }

};

class Tokenizer{
    std::fstream file;
    size_t prevPos;
    public:
    Tokenizer(std::string fileName){
        file.open(fileName, std::ios::in);
        if(!file.good()){
            std::cout << "File open error" << std::endl;
        }
    }
    auto getWithoutWhiteSpace(){
        char c = ' ';
        while((c==' '||c=='\n')){
            file.get(c); // check
            if((c==' '||c=='\n')&&file.eof()){
                throw std::logic_error("Ran out of tokens");
            }
        }
        // std::cout << "NOWHITE: " << c << std::endl;
        return c;
    }
    auto getToken(){
        // string buf;
        char c;
        if(file.eof()){
            std::cout << "Exhaused tokens" << std::endl;
            // throw std::exception("Exhausted tokens");
        }
        prevPos = file.tellg();
        c = getWithoutWhiteSpace();
        
        
        
        struct Token token;
        if(c=='"'){
            token.type = TOKEN::STRING;
            token.value = "";
            file.get(c);
            while(c!='"'){
                token.value+=c;
                file.get(c);
            }
        }
        else if(c=='{'){
            token.type = TOKEN::CURLY_OPEN;
        }
        else if(c=='}'){
            token.type = TOKEN::CURLY_CLOSE;
        }
        else if(c>='0'&&c<='9'){
            //Check if string is numeric
            token.type = TOKEN::NUMBER;
            token.value = "";
            token.value+=c;
            while((c>='0'&&c<='9')||c=='.'){
                file.get(c);
                token.value+=c;
            }

        }
        else if(c=='[')
        {
            token.type = TOKEN::ARRAY_OPEN;
        }
        else if(c==']')
        {
            token.type = TOKEN::ARRAY_CLOSE;
        }
        else if(c==':'){
            token.type = TOKEN::COLON;
        }
        else if(c==','){
            token.type = TOKEN::COMMA;
        }
        return token;
    }

    auto hasMoreTokens(){
        return !file.eof();
    }

    void rollBackToken(){
        file.seekg(prevPos);
    }

    
    /*
        "": String begin?
        {}: Object
        []: List
        num: number
        
     */
};