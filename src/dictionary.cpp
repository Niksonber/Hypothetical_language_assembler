#include "../include/dictionary.hpp"

namespace dictionary {
    bool reserved(std::string word){
        return (commands   . find(word) != commands   . end())
            && (directives . find(word) != directives . end());
    };

    bool illegal(char c){
        return (c < 'A' || c > 'Z') // Non-Alphabetic
            && (c < '0' || c > '9') // Non-Numeric
            && (c != ':')           // Non-Colon
            && (c != ';')           // Non-Comment
            && (c != '_');          // Non-Underscore
    };
}