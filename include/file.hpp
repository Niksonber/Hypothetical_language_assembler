#include <iostream>
#include "typedefs.hpp"


bool  from_file(std::string, vector_of_strings*, std::string = ".asm");
bool  to_file  (vector_of_strings, std::string, std::string);

// Useful Overloads
bool to_file(std::string, std::string, std::string);
bool to_file(std::vector<int>, std::string, std::string);