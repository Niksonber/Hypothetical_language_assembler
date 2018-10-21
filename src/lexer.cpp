#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

#include "../include/lexer.hpp"



bool readline(std::string line, std::vector<Token>* tokens, unsigned int lineindex){
    if (line.empty())   return false;
    tokens->clear();

    std::string consumable(line);


    Token t;
    while (!(t = grab_token(&consumable)).empty())
        tokens->emplace_back(Token(t.text, lineindex)); // t.line is 0 here


    #ifdef DEBUG_LEXER_PRINT_TOKENS
        std::cout << "[lexer]: ";
        for (auto t : (*tokens))
            std::cout << t.text << " ";
        std::cout << std::endl;
    #endif

    return true;
}


Token grab_token(std::string* line){
    Token result;
    if (line->empty()) return result;

    trim(line);

    char firstchar = line->front();
    if (firstchar == ';'){
        return result;
    }

    bool hex    = (line->size() > 2) && (hexa(line->substr(0, 2)));
    bool number = numeric    (firstchar) && !hex;
    bool word   = alphabetic (firstchar);
    bool symbol = symbolic   (firstchar);

    unsigned short int length = 0;
    
    // if (hex) std::cout << "HUASIHF" << std::endl;
    if (hex){
        result = "";
        result += line->substr(0, 2);
        line->erase(0, 2);
    }

    for (auto c : (*line)){

        if (hex    and !hexa(c))                       break;
        if (word   and !alphabetic(c) and !numeric(c)) break;
        if (number and !numeric(c))                    break;

        length++;
        if (symbol) break; // Symbols are only 1 char long
    }

    result += line->substr(0, length);
    line->erase(0, length);
    return result;
}

void trim(std::string* str){
    if (str->empty()) return;

    unsigned int count = 0;
    for (auto c : (*str)) if (c == ' ' || c == '\t') count++;

    str->erase(0, count);
}



/* Checks for lexical errors:
    1. Token is label and reserved word.
    2. Non-const token starts with number;
*/
// TODO: pass last token as param
bool lex_error(std::string t){ // , std::string prev = "")
    using namespace dictionary;

    // 3. Token contains illegal characters
    // for(char c : t)
    for(unsigned int i=0; i<t.size(); i++)
        if(illegal(t[i]))
            return true;

    // 1. Token is label and reserved word.
    if (t[t.size() - 1] == ':')
        if (reserved(t))
            return true;

    // 2. Non-param (CONST, SPACE) token starts with number;
    if (t[0] > '0' && t[0] < '9')
    // if (prev != "SPACE" && prev != "CONST")
        return true;
    
    return false;
}


// lang.h
const static std::string symbols = ":;,+";

bool alphabetic (char c){
    return (c >= 'A' && c <= 'Z') || (c == '_'); // Special case for the underscore symbol
}
bool alphabetic (std::string s){
    for (auto c : s)
        if (!alphabetic(c))
            return false;
    return true;
}

bool symbolic   (char c){
    return (c == ':') || (c == ';') || (c == ',');
    // return (symbols.find(c) != symbol.end());
}
// Modify this to include the '-' character for negative numbers
bool numeric    (char c){
    return (c >= '0' && c <= '9');
}

// Modify this account for hexadecimals
bool numeric    (std::string s){
    for (auto c : s)
        if (!numeric(c))
            return false;
    return true;
}

bool hexa(char c){
    return numeric(c) || (c >= 'A' && c <= 'F');
}

bool hexa(std::string s){
    auto c = s.begin();

    // std::cout << "[hexa] String: "   << s  << std::endl;

    if (*c != '0') return false;
    // std::cout << "[hexa] 1nd char: " << *c << std::endl;
    ++c;

    if (*c != 'X') return false;
    // std::cout << "[hexa] 2st char: " << *c << std::endl;
    ++c;

    while (c != s.end())
        if (hexa(*c)) c++;
        else return false;

    return true;
}












// ----------------------------------------------------------------------------- DEPRECATED
Token extract_label(std::vector<Token> line){
    if (!line.empty()){
        Token first_token = line.front();
        
        if (first_token.text.back() == ':'){
            first_token.text.erase(first_token.text.size() - 1);
            return first_token;
        }

        if (line.size() > 1 && (line[1].text == ":"))
            return first_token;
    }
    return Token();
}

std::vector<Token> skip_label(std::vector<Token> line){
    bool ok = false;
    Token first_token;

    while(!ok){
        ok = true;

        if (line.empty()) return line;

        first_token = line[0];

        if (first_token == ":" || first_token.text.back() == ':'){ // DO NOT ACCEPT
            ok = false;
            line.erase(line.begin());
        };

        if (line.size() > 1 && line[1] == ":"){
            ok = false;
            line.erase(line.begin() + 1);
        };
    }

    return line;
}

// ----------------------------------------------------------------------------------------