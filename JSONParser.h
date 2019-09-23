#define TRANSITION(TOKEN, STATE, NEXT_STATE) transitions[make_pair(Token::TOKEN,State::STATE)] = NEXT_STATE

enum class State{
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

class JSONParser{
    std::fstream file;
    std::stack<State> currentStates;
    std::map<std::pair<TOKEN, State>, State> transitions;
    JSONObject& root;

    JSONParser(){
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
        TRANSITION(STRING, INSIDE_GLOBAL, END);
        TRANSITION(CURLY_L, INSIDE_GLOBAL, INSIDE_OBJECT);
        TRANSITION(NUMBER, INSIDE_GLOBAL, END);
        TRANSITION(ARRAY_OPEN, INSIDE_GLOBAL, INSIDE_ARRAY);
        TRANSITION(STRING, INSIDE_OBJECT, HAS_KEY);
        TRANSITION(CURLY_R, INSIDE_OBJECT, POP);
        TRANSITION(COLON, HAS_KEY, END_KEY);
        TRANSITION(CURLY_L, END_KEY, INSIDE_OBJECT);
        TRANSITION(ARRAY_OPEN, END_KEY, INSIDE_ARRAY);
        TRANSITION(STRING, END_KEY, END_VALUE);
        TRANSITION(NUMBER, END_KEY, END_VALUE);
        TRANSITION(COMMA, END_VALUE, VALID_OBJECT);
        TRANSITION(STRING, VALID_OBJECT, HAS_KEY);
        TRANSITION(STRING,INSIDE_ARRAY, ARRAY_VALUE_START);
        TRANSITION(NUMBER, INSIDE_ARRAY, ARRAY_VALUE_START);
        TRANSITION(CURLY, INSIDE_ARRAY, INSIDE_OBJECT);
        TRANSITION(ARRAY_OPEN, INSIDE_ARRAY, INSIDE_ARRAY);
        TRANSITION(COMMA,ARRAY_VALUE_START, INSIDE_ARRAY);
        TRANSITION(ARRAY_CLOSE,ARRAY_VALUE_START, POP);

        /*
            Check right order of pops
         */
        


        //on final pop, if inside global, then end
        
        return process(file);
    }

    State transitionFunction(TOKEN token){
        /*
            Need to pop have a pop
         */
        if(transitions.find(std::make_pair(token, currentState.top()))==transitions.end()){
            return State::INVALID;
        }
        return transitions[std::make_pair(token, currentState.top())];
    }

    struct transition(TOKEN token){
        //ON transition
        State nextState = transitionFunction(token);
        vector<State> popStates = {State::END_KEY, State::END_VALUE,State::HAS_KEY, State::ARRAY_VALUE_START};
        if(nextState==State::INVALID){
            throw std::exception("Invalid JSON");
        }
        if(std::any_of(popStates.begin(), popStates.end(), [](State a){return a==currentStates.top()}){
            currentStates.pop();
        }
        if(nextState==State::POP){
            currentStates.pop();
        }

        //THOSE TOKENS in between need to be popped
        
        if(currentState.top()==STATE::END_KEY){
            
        }
    }

    void parse(){
        std::string key;
        while(!tokenizer.hasMoreTokens()){
            Token token = tokenizer.getToken();
            transition(token);
            switch(currentState.top()){
                case STATE::HAS_KEY:{}.
                case STATE::ST
            }
            if(currentState.top()==STATE::HAS_KEY){
                key = token.value;
            }


        }
        //assert token not valid
    }

    void parseObject(){

    }
    void parseString(){

    }
    void parseNumber(){
        
    }

    void parseList(){

    }
    auto process(file){

    }
};
