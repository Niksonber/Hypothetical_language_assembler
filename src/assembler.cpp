#include "../include/assembler.hpp"

#ifdef DEBUG_ASSEMBLER_PRINT_HEADERS        
void printheader(program prog){
    std::string mask;
    std::string code;
    std::string text;

    for (auto relative : prog.relative) mask += (relative ? '1' : '0');

    for (auto cmd : prog.code.statements){
        text += cmd.exp.token.text;
        text += ' ';

        for (auto p : cmd.params){
            text += p.exp.token.text;
            text += ' ';
        }    
    }

    for (auto cmd : prog.code.statements){
        code += std::to_string(cmd.exp.value);
        code += ' ';

        for (auto p : cmd.params){
            code += std::to_string(p.exp.value);
            code += ' ';
        }    
    }
    

    std::cout << "\n[assembler] Module: "
              << p.name
              << "\n[assembler] Header: "
              << "\n - Size: "
              << p.size
              << "\n - Mask: "
              << mask
              << "\n - Text: "
              << text
              << "\n - Code: "
              << code
              << "\n - Usage Table: "
              << "\n - Definition Table: "
              << std::endl;
}
#endif // DEBUG_ASSEMBLER_PRINT_HEADERS

std::string assemble(program prog, bool modular) {
    
    std::string output;

    // Modular: Structure modules and tables and stuff
    if (modular){
        // Header: Name
        output += prog.name;
        output += '\n';

        // Header: Size
        output += std::to_string(prog.size);
        output += '\n';

        // Header: Relative Bitmask
        for (auto relative : prog.relative) output += (relative ? '1' : '0');
        output += '\n';

        // Header: Definition Table
        for (auto def : prog.dt){
            output += def.first;
            output += ' ';
            output += std::to_string(def.second);
            output += ' ';
        }
        output += '\n';

        // Header: Use Table
        for (auto use : prog.ut){
            output += use.first;
            output += ' ';
            output += std::to_string(use.second);
            output += ' ';
        }
        output += '\n';
    }

    // Code (this stuff is important)
    for(auto& cmd : prog.code.statements) {
        output += std::to_string(cmd.exp.value);
        output += ' ';

        for(auto& p : cmd.params){
            output += std::to_string(p.exp.value);
            output += ' ';
        }
    }
    

    

    #ifdef DEBUG_ASSEMBLER_PRINT_OUTPUT
        std::string text;

        for (auto cmd : prog.code.statements){
            text += cmd.exp.token.text;
            text += ' ';

            for (auto p : cmd.params){
                text += p.exp.token.text;
                text += ' ';
            }    
        }

        std::cout << "\n[assembler] Generated output for "
                  << (modular ? "module " + prog.name : "program")
                  << ":\n"
                  << output
                  << "\n[assembler] Commands are: \n"
                  << text
                  << std::endl
                  << std::endl;
    #endif // DEBUG_ASSEMBLER_PRINT_OUTPUT

    #ifdef DEBUG_ASSEMBLER_PRINT_HEADERS
        printheader(prog);
    #endif // DEBUG_ASSEMBLER_PRINT_HEADERS

    return output;
}