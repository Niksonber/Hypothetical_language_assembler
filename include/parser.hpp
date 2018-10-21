#pragma once
#include <iostream>
#include <map>

#include "typedefs.hpp"
#include "dictionary.hpp"

#include "error.hpp"

#include "model.hpp"


// Helpers
bool   declared(std::string, symbol_table);
symbol retrieve(std::string, symbol_table);

bool declared(std::string, def_table);
int  retrieve(std::string, def_table);

void print_def_table (def_table,    std::string = "");
void print_sym_table (symbol_table, std::string = "");
// bool add_entries     (def_table, def_table*);


// Core Function
bool parse (vector_of_tokens*, program*, bool = false);

/** 1st pass
 * input:
 *  1. Code, as tokens
 *  2. Local Symbol Table pointer
 * 
 * returns:
 *  Definition Table (exported variables)
 */
bool first_pass(program*, vector_of_tokens*, bool = false);

/** 2st pass
 * input:
 *  1. Reference to output ast
 *  2. Code, as tokens, to be parsed into the ast
 *  3. Local Symbol Table
 *  4. Global Definition Table, to check for cross-references
 * 
 * returns:
 *  Usage Table (imported and local symbols)
 */
bool second_pass(program*, vector_of_tokens&, bool = false);

bool astcheck   (ast&, vector_of_strings&);

expression parseexp  (Token, symbol_table& st);