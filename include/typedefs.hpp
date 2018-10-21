#pragma once
#include <fstream>
#include <vector>


#include "error.hpp"

#define vector_of_strings std::vector<std::string>
#define vector_of_tokens  std::vector<Token>

struct Token {
  Token(std::string text = "", int line = 0)
    : text(text), line(line) {};

    // Main structure
  std::string text;
  int line;

    // Operators for sanity
  template<typename T> void operator+= (T rhs){ text += rhs; }
  template<typename T> void operator=  (T rhs){ text = rhs;  }
  bool operator== (std::string rhs){ return text == rhs; }
  bool operator!= (std::string rhs){ return text != rhs; }
  bool empty (){ return text.empty() || text == ""; }

    // Are these necessary?
  bool label_equ   = false;
  bool label_macro = false;
  bool label       = false;

  void flag(ERRCODE c){
      this->haserror = true;
      this->errcode  = c;
  }
  bool    haserror = false;
  ERRCODE errcode;
};

// TRAB2
struct Header {
    std::string   name;
    unsigned int  size;
    std::string   bitmask;

    void print(){
        std::cout << "Header: \n" 
                  << name << '\n' 
                  << size << '\n'
                  << bitmask
                  << std::endl;
    }
};