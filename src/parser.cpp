#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include "../include/debug.h"
#include <cstdlib>


// WIP
bool parse (vector_of_tokens* source, program* output, bool modular){
    if (output == nullptr) output = new program;

    // Copy (safety) and treat input code
    vector_of_tokens* code = new vector_of_tokens;
    vector_of_tokens  buffer;

    for (auto origtoken : (*source)){
        if (!readline(origtoken.text, &buffer, origtoken.line))
            continue;    

        code->insert(code->end(), buffer.begin(), buffer.end());
    }


    // First-Pass Modules
    if (!first_pass  (output, code,  modular)) return false;

    // Second Pass Modules
    if (!second_pass (output, *code, modular)) return false;

    //delete some stuff
        
    return true;
}


// TODO: Change to unsigned int, returning size of code read.
bool first_pass(program* prog, vector_of_tokens* source, bool modular){
    using namespace dictionary;
    

    // Loop Setup
    int  pc = 0; // Parse-Relevant
    auto it = source->begin();
    auto exported = new vector_of_strings;


    /** FIXME: Error detection here is useless, because we delete the tokens
     * one possible solution is to keep an array of error-flagged tokens,
     * and check them at the end of the first pass.
     */

    // Parsing Loop
    unsigned short int rem = 0; // remove range
    while(it != source->end()){

        // source consumption, centralized
        while (rem --> 0){
            it = source->erase(it);
            if (it == source->end())
                break;
        }
        auto next = it + 1;
        rem = 0;

        if (it == source->end()) break; // EOF
        if (it->text == "CONST") it++; /// Bypass
        if (it == source->end()) break; // EOF

        // Section Found : TODO: Make Checks
        if (it->text == "SECTION"){
            // rem+=2;
            rem++;
            if (next == source->end()) { /* error */ } // EOF
            else if (next->text == "TEXT") { rem++; }
            else if (next->text == "DATA") { rem++; }
            else {
                // ERROR: UNKNOWN SECTION TYPE
                std::cout << "FUICK: " << next->text << std::endl;
                exit(0);
            }
            continue;
        }

        // Checking for BEGIN tag in modularized files
        // if (pc == 0 && modular){
        //     if (next == source->end()) { rem++; continue; /* error */ }
        //     if (next->text != ":")   { rem++; continue; /* error */ }
        // }

        if (it->text == "END"){
            if (next != source->end()) {
                // ERROR: Code beyond END tag has been ignored
            }
            source->erase(it, source->end());
            break;
        }

        if (it->text == "BEGIN"){
            // It never gets here
            // ERROR: Either not modular, or misplaced BEGIN tag
            rem++;
            continue;
        }

        // EXTERN: Error, if not processed inside label context
        if (it->text == "EXTERN"){
            if (!modular) { /* ERROR */ }

            rem++;
            continue;
        }

        // PUBLIC: insert into definition table
        if (it->text == "PUBLIC"){
            if (!modular) { /* ERROR */ }

            rem++;
            if (next == source->end()) break; // EOF
            
            // for(auto d : *exported)
            //     if (d == next->text){
            //         it->flag(LABEL_MULTIEXPORT);
            //         break;
            //     }

            exported->push_back(next->text);
            rem++;
            
            // it += 2;
            continue;
        }

        
        // LABEL Processing
        if (next != source->end() && next->text == ":"){
            rem++;
            
            next = source->erase(next, next + 1);

            // Check for errors
            if (declared(it->text, prog->st)) it->flag(LABEL_REDECLARED);
            else if (reserved(it->text))      it->flag(LABEL_RESERVED);
            else {

                if(next == source->end()) /* error */
                    break;
                
                // USE THIS!!
                // it->label = true;
                
                // EXTERN: inform usage of external symbol (to usage_table)
                if (next->text == "EXTERN"){
                    if (!modular) { /* ERROR */ }
                    prog->st.push_back(symbol(it->text, 0, true));
                    rem++;
                    continue;
                }
                
                // BEGIN
                if (next->text == "BEGIN"){
                    if (!modular) { /* ERROR */ }
                    if (pc != 0)  { /* ERROR */ }
                    prog->name = it->text;
                    prog->st.push_back(symbol(it->text, 0));
                    exported->push_back(it->text);
                    rem++;
                    continue;
                }
                
                // Regular Label
                prog->st.push_back(symbol(it->text, pc));
            }

            continue;
        }

        #ifdef DEBUG_PARSER_FIRST_PASS
            std::cout << "[parser : 1st pass] "
                      << pc
                      << "\t: "
                      << it->text
                      << std::endl; 

        #endif // DEBUG_PARSER_FIRST_PASS

        it++;
        pc++;
    }


    #ifdef DEBUG_PARSER_SYMBOL_TABLE
        print_sym_table(prog->st);
    #endif // DEBUG_PARSER_SYMBOL_TABLE

    for (auto entry : *exported)
        if (prog->dt.find(entry) == prog->dt.end()) // Symbol not previously defined
            if (declared(entry, prog->st)) // Symbol not locally undefined
                prog->dt[entry] = retrieve(entry, prog->st).value;
            else {
                // ERR: Cannot set unknown symbol to public
            }
        else{
            // ERR: Multiple Exports of same symbol
        }
    
    delete exported;

    return true;
};

bool second_pass(program* prog, vector_of_tokens& source, bool modular){
    using namespace dictionary;
    if (prog == nullptr) return false;
    if (source.empty())  return false;

    unsigned short int args = 0;
    int pos = 0;

    auto it = source.begin();
    while(it != source.end()){
        expression e = parseexp(*it, prog->st);
        e.position   = pos;
        prog->relative.push_back(e.relative);   // Relative


        // -------------------------------------------- <Treating Directives>
        if (e.token == "CONST"){
            ++it;

            bool neg = (it->text[0] == '-');

            if (neg) ++it;

            if (it == source.end()) { // ERROR: UNEXPECTED EOF
                e.flag(UNEXPECTED_EOF);
                prog->code << ast_node(e);
                continue;
            }

            if (!numeric(it->text)){
                // ERROR!
                e.flag(ILLEGAL_PARAM);
                prog->code << ast_node(e);
                continue;
            }

            /*
                
            // Checking for
            int num = std::stoi(it->text);
            if (num == 0)
                if ((++it) != source.end())
                if (it->text[0] == 'x')
                if ()



            e.value = neg ? (- num) : num;
            */

            e.value = neg ? (- std::stoi(it->text)) : std::stoi(it->text);
            prog->code << ast_node(e);

            ++it;
            ++pos;
            continue;
        }


        else if (e.token == "SPACE"){
            ++it;
            
            args = 1; 
            if (it != source.end() && numeric(it->text)){
                args = std::stoi(it->text);
                ++it;
            }
            
            while (args --> 0){
                prog->code << ast_node(expression(e));
                prog->relative.push_back(false);
                ++pos;
            }

            continue;
        }

        // -------------------------------------------- </Treating Directives>

        // Regular Statement Parsing
        ast_node statement(e);

        if (it == source.end()) break;
        ++it;
        ++pos;

        // Statemente Arguments
        args = e.param_count;
        while (args > 0){
            if (it == source.end()) break; // ERR unexpected EOF

            expression param = parseexp(*it, prog->st);
            
            prog->relative.push_back(param.relative);
            
            // Error: Parameter is reserved word
            if (reserved(param.token.text))
                statement.exp.flag(ILLEGAL_PARAM);


            
            // Solving Reference
            if (declared(param.token.text, prog->st)){
                auto s    = retrieve(param.token.text, prog->st);
                auto next = it + 1;

                // Externally Defined => Usage Table
                if (s.ext) prog->ut.push_back({param.token.text, pos});
                // Locally Defined => Substitute!
                else param.value = s.value;

                // if (s.ext || declared(param.token.text, prog->dt)); 

                // Treating Vector Offset Access
                if (next != source.end())
                    if (next->text == "+")
                    if ((++next) != source.end())
                    if (numeric(next->text)){
                        param.token.text += "+";
                        param.token.text += next->text;
                        param.value += std::stoi (next->text);
                        it += 2;
                    }

            }
            else {
                param.flag(UNRESOLVED_SYM);
            }



            // Emplacing parameter, loop
            statement.params.push_back(param);
            
            ++it;
            ++pos;
            --args;
        };

        prog->code << statement;
    }

    prog->size = pos;

    return true;
}














// ----------------------------------------------------------------------- HELPERS

expression parseexp(Token tok, symbol_table& st) {
    using namespace dictionary;
    expression e(tok);
    e.token.line = tok.line;

    // Variable
    for (auto s : st) if (s.text == tok.text){
        e.value    = s.value;
        e.relative = true;
        return e;
    }
    
    // Command
    if (commands.find(tok.text) != commands.end()){
        command c      = commands.find(tok.text)->second;
        e.data.command = c.name;
        e.value        = c.opcode;
        e.param_count  = c.param_count;
        return e;
    }

    // Directive @CATCH: Technically, directives should not be parsed into expressions, right?
    if (directives.find(tok.text) != directives.end()){
        directive d      = directives.find(tok.text)->second;
        e.data.directive = d.name;
        e.param_count    = d.param_count;
        return e;
    }

    return e;
}


bool astcheck   (ast& code, vector_of_strings& orig){
    unsigned short int errcount = 0;

    bool lineerr = false;

    for (auto s : code.statements) {
        if ((lineerr |= s.exp.haserror()) == true) {
            errcount++;
            error::print(s.exp.errcode(), s.exp.token.line);
        }

        for (auto p : s.params)
            if ((lineerr |= p.exp.haserror()) == true) {
                errcount++;
                error::print(p.exp.errcode(), p.exp.token.line);
            }

        if (lineerr) {
            std::cout << "\t (" 
                      << s.exp.token.line
                      << ") "
                      << orig[s.exp.token.line];
                      
            for (auto p : s.params)
                std::cout << orig[p.exp.token.line] << " ";

            std::cout << std::endl  << std::endl;
        }


        lineerr = false;
    }

    // @DELETE
    if (errcount > 0)
        std::cout << "[parser] Detected "
                  << errcount 
                  << " errors in "
                  << code.statements.size()
                  << " lines."
                  << std::endl;

    return (errcount == 0);
}

void print_def_table(def_table table, std::string modname){
    std::string tag = (modname.empty())
                    ? "[parser] "
                    : "[parser : " + modname + "] ";
    if (table.empty()) 
        std::cout << tag
                  << "No prog->dt Table Loaded"
                  << std::endl;

    else {          
        std::cout << tag
                  << "Loaded prog->dt Table"
                  << std::endl;

        for (auto& entry : table)
            std::cout << "  "
                      << entry.first 
                      << ": " 
                      << entry.second 
                      << std::endl;
    }
}

void print_sym_table(symbol_table table, std::string modname){
    std::string tag = (modname.empty())
                    ? "[parser] "
                    : "[parser : " + modname + "] ";
    if (table.empty()) 
        std::cout << tag
                  << "No Symbol Table Loaded"
                  << std::endl;

    else {          
        std::cout << tag
                  << "Loaded Symbol Table"
                  << std::endl;

        for (auto& entry : table)
            std::cout << "  "
                      << entry.text
                      << ": " 
                      << entry.value
                      << (entry.ext ? " (external) " : "")
                      << std::endl;
    }
}

// bool add_entries(def_table table, def_table* gdt){
//     bool err = 0;
//     if (gdt == nullptr) gdt = new def_table;

//     for (auto entry : table){
//         err |= (gdt->find(entry.first) == gdt->end());

//         if (err) break;
//         else (*gdt)[entry.first] = entry.second;
//     }

//     return !err;
// }


bool declared(std::string text, symbol_table st){
    for (auto s : st)
        if (s.text == text)
            return true;
    return false;
}

bool declared(std::string text, def_table dt){
    if (dt.find(text) != dt.end()) return true;
    return false;
}

symbol retrieve(std::string text, symbol_table st){
    for (auto s : st)
        if (s.text == text)
            return s;
    return symbol("err", -1, false);
}

int retrieve(std::string text, def_table dt){
    if (dt.find(text) != dt.end()) return dt[text];
    return -1;
}




























/*
ast parse(vector_of_tokens code_safe){
    
    vector_of_tokens code;

    // Creating local, treated copy of code
    vector_of_tokens buffer;
    for (auto origtoken : code_safe){
        if (!readline(origtoken.text, &buffer, origtoken.line)) continue;
        
        code.insert(code.end(), buffer.begin(), buffer.end());
    }

    // Can go to beginning, as code_safe
    if (code.empty()) return ast();

    // 1st pass: Symbol Table, Sanitize
    def_table st;
    if (!first_pass(&code, &st)) exit(-5);

    // 2nd pass: Build AST
    ast parsed; // = second_pass(code, st)
    if (!second_pass(&parsed, code, st)) exit(-5);

    // Function
    #ifdef DEBUG_PARSER_AST
        std::vector<ast_node> statements = parsed.statements;
        for(auto& s : statements){
            std::cout << "[parser : 2nd pass] "
                      << s.exp.position
                      << "\t: "
                      << s.exp.token.text;

            for(auto& p : s.params)
                std::cout << ' '
                          << p.exp.token.text;

            std::cout << std::endl;
        }
    #endif // DEBUG_PARSER_AST

    return parsed;
};
*/