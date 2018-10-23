#include <cstdlib>
#include "../include/debug.h"

#include "../include/program.hpp"
#include "../include/prep.hpp"
#include "../include/parser.hpp"
#include "../include/assembler.hpp"
#include "../include/file.hpp"

int run(std::string flag, vector_of_strings files){
    // Read Flags
    if (flag.length() < 2) exit(-3);
    if (files.size()  < 1) exit(-4);

    bool modular = (files.size() > 1);
    char mode    = flag[1];

    
    #ifdef DEBUG_PROGRAM_INPUT_FILENAMES
        std::cout << "[program] input files:" << std::endl;
        for (auto f : files)
            std::cout << "\t- "
                      << f
                      << std::endl;
    #endif // DEBUG_PROGRAM_INPUT_FILENAMES
    
    // Gigantic loop
    for (auto file : files)
        process(file, mode, modular);
        
    return 0;
}




bool process(std::string filename, char mode, bool modular){
    
// -------------------------------------------------------------------- Preprocess Files
    bool BEGIN = false;
    bool END = false;
    vector_of_strings prepped;
    vector_of_strings temp;
    vector_of_tokens* tokenized = new vector_of_tokens; 

    // Ties input name to output name
    std::string output(filename);

    
    if (from_file(filename, &temp)){
        prepped.clear();
        preprocess(temp, &prepped, mode!='p', tokenized);
    
    }

    else{
        std::cout << "Problem reading file: "
                << filename
                << ". Aborting Execution"
                << std::endl;
        exit(-5);
    }
    // Flag-controlled Outputs
    // 1. If, Equ Expanded
    if (mode == 'p') {
        to_file(prepped, output, ".pre");
        return 0;
    }

    for (vector_of_tokens::iterator it = tokenized->begin() ; it != tokenized->end(); ++it){
        if (it->text.compare("BEGIN") == 0){
            BEGIN = true;
        }else if (it->text.compare("END") == 0){
            END = true;
        }
    }
    if(modular == false){
        if(BEGIN == true || END == true){
            std::cout<<"\033[04;91m"<<"\nPresença de BEGIN ou END sendo um arquivo só.\n"<<"\033[00m";
            exit(-5);
        }
    }else{
        if(BEGIN == false || END == false){
            std::cout<<"\033[04;91m"<<"\nAusencia de BEGIN ou END em um dos dois arquivos.\n"<<"\033[00m";
            exit(-5);
        }
    }

// -------------------------------------------------------------------- Parse Modules
    program* parsed = new program();

    if (!parse(tokenized, parsed, modular)) exit(-6);
    if (!astcheck(parsed->code, prepped))  exit(-5);    

// -------------------------------------------------------------------- Assemble Modules
    std::string as = assemble(*parsed, modular);
    // std::cout << as <<std::endl;
    if (!to_file(as, output, ".o")) exit(-10);

    // Cleanup
    prepped.clear();
    temp.clear();
    delete tokenized;
    delete parsed;
    
    return true;
}