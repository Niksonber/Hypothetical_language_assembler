#include <iostream>
#include <cstdlib>

// Program
#include "../include/program.hpp"

/* Args
 * [1]: Operacao (flag)
 * [2]: Arquivo de entrada (texto, sem extensao (infere-se .asm))
 * [3]: Arquivo de saida (sem extensao)
 *
 * Flags:
 * -p (preprocessador)  => .pre (processa apenas expressoes IF e EQU)
 * -m (macros)          => .mcr (processa macros)
 * -o (montagem)        => .o   (monta o codigo fonte dado)
 *
 */

// MAIN
int main(int argc, char *argv[]){
    // int returncode;
    vector_of_strings args;
    vector_of_strings files;


    for(int i = 1; i < argc; i++) // skip program name
        args.push_back(argv[i]);

    // if (args.size() < 2) exit(-1);

    std::string mode   = args.front();

    // Prettier ways to acheive this
    for (unsigned int i = 1; i < args.size(); i++) // ignore output filename
        files.push_back(std::string(args[i]));

    args.clear();

    return run(mode, files);
}
