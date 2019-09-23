#include<fstream>
#include<vector>
#include<map>
#include<stdexcept>
#include<stack>
enum class TOKEN{
    CURLY_BRACE,
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
};

class Tokenizer{
    std::fstream file;
    Tokenizer(std::string fileName){
        file.open(fileName, std::ios::in);
    }
    auto getWithoutWhiteSpace(){
        char c = ' ';
        while(c==' '&&!file.eof()){
            file.get(&c); // check
        }
        return c;
    }
    auto getToken(){
        // string buf;
        char c;
        if(file.eof()){
            throw std::exception("Exhausted tokens");
        }
        c = getWithoutWhiteSpace()
        struct Token token;
        if(c=='"'){
            token.type = TOKEN::STRING;
            token.value = "";
            file.get(&c);
            while(c!='"'){
                token.value+=c;
                file.get(&c);
            }
        }
        else if(c=='{'){
            token.type = TOKEN::CURLY_BRACE;
        }
        else if(c>='0'&&c<='9'){
            token.type = TOKEN::NUMBER;
            token.value = "";
            token.value+=c;
            while((c>='0'&&c<='9')||c=='.'){
                file.get(&c);
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
    /*
        "": String begin?
        {}: Object
        []: List
        num: number
        
     */
};