#include<iostream>
#include "JSONParser.h"

int main(int argc, char* argv[]){
    std::string filename = argv[1];
    JSONParser parser(filename) ;
    parser.parse();

}

//How to make this recursive?








/*
    Container to hold a templated type?
 */