#include <iostream>
#include <map>

#include "../include/typedefs.hpp"
#include "debug.h"


bool read_source_line(std::string, vector_of_tokens*,int*);
void preprocess(vector_of_strings&, vector_of_strings*, bool,vector_of_tokens*);
bool treating_if(vector_of_tokens*);
void update_label_flags(vector_of_tokens*);
void label_tokens(vector_of_tokens*);
void if_equ_output(vector_of_tokens*,vector_of_strings*);
void separe_copy_tokens(vector_of_tokens*);
void section_erros(vector_of_tokens*);
void printing_errors(vector_of_tokens*);
void double_label_line(vector_of_tokens*);
void label_redeclared(vector_of_tokens*);
void lexical_error(vector_of_tokens*);