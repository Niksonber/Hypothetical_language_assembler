#include <iostream>
#include "debug.h"
#include "typedefs.hpp"

#include "dictionary.hpp"

/* Token Type:

 */
// typedef std::string token;

bool  readline   (std::string, vector_of_tokens*,  unsigned int = 0);
Token grab_token (std::string*);
bool  lex_error  (std::string);

vector_of_tokens skip_label   (vector_of_tokens);
Token            extract_label(vector_of_tokens);


bool alphabetic (char);
bool numeric    (char);
bool hexa       (char);
bool symbolic   (char);

bool alphabetic (std::string);
bool numeric    (std::string);
bool hexa       (std::string);

void trim(std::string*); // Removes Spaces