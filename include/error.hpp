#pragma once 
#include <iostream>
#include <map>

enum ERRCODE {
    // Lexical
    ILLEGAL_NAME,
    UNKNOWN_SEP,
    LABEL_REDECLARED,

    // Semantic
    DIF_SECTION,
    DOUBLE_LABEL_LINE,
    REDECLARATION,

    // Syntactic
    UNRESOLVED_SYM,
    UNEXPECTED_EOF,
    LABEL_RESERVED,
    LABEL_MULTIEXPORT,
    MISSING_PARAM,
    ILLEGAL_PARAM
};

namespace error {

    void warn  (ERRCODE, int = -1, std::string = "");
    void error (ERRCODE, int = -1, std::string = "");
    void print (ERRCODE, int = -1, std::string = "");

    static std::map<ERRCODE, std::string> lex = {
        { ILLEGAL_NAME,     "Illegal token: "          },
        { UNKNOWN_SEP,      "Unknown symbol: "         }
        
    };

    static std::map<ERRCODE, std::string> sema = {
        { DIF_SECTION, "Section different than TEXT and DATA: " },
        { DOUBLE_LABEL_LINE, "Two or more labels in the same line: " },
        { REDECLARATION, "Redeclaration of the label: " }
    };

    static std::map<ERRCODE, std::string> syn = {
        { UNRESOLVED_SYM,   "Could not resolve symbol: "             },
        { UNEXPECTED_EOF,   "Unexpected end of file."                },
        { LABEL_RESERVED,   "Label name is reserved word: "          },
        { MISSING_PARAM,    "Expression is missing parameter(s): "   },
        { ILLEGAL_PARAM,    "Illegal parameter for expression: "     },
        { LABEL_MULTIEXPORT,"Label declared public more than once: " },
        { LABEL_REDECLARED, "Redeclaration of label: " }
    };
}