#include "../include/file.hpp"
#include "../include/debug.h"


void print_writing_data(std::string filename, std::string extension){
    std::cout << std::endl 
              << "[file]: Reading back " 
              << filename 
              << extension 
              << std::endl;

    vector_of_strings readback;
    if (from_file(filename, &readback, extension))
        for (std::string line : readback)
            std::cout << line
                      << std::endl;

    else std::cout << "[file] Error opening readback file: " 
                  << filename 
                  << extension 
                  << std::endl;
                  
    std::cout << std::endl;
}

// Single-string overload
bool to_file(std::string data, std::string filename, std::string extension){
    std::ofstream output;
    output.open ((filename + extension).c_str());
    if (!output) return false;

    output << data;

    output.close();
    // Debug Printings
    #ifdef DEBUG_FILE_PRINTDATA
        print_writing_data(filename, extension);
    #endif
    return true;
}

bool to_file(std::vector<int> data, std::string filename, std::string extension){
    vector_of_strings bin;
    for (auto number : data) bin.push_back(std::to_string(number) + " ");

    return to_file(bin, filename, extension);
}


// Source code overload :: unused
bool to_file(vector_of_strings data, std::string filename, std::string extension){
    // Write to File
    std::ofstream output;
    output.open ((filename + extension).c_str());

    if (!output) return false;

    for (auto& line : data) output << line;

    output.close();

    // Debug Printings
    #ifdef DEBUG_FILE_PRINTDATA
        print_writing_data(filename, extension);
    #endif

    return true;
}


bool from_file(std::string filename, vector_of_strings* res, std::string ext){
    std::ifstream input((filename + ext).c_str());
    if (!input) return false;

    std::string temp;
    while(std::getline(input, temp)){
        // Debug Printings
        #ifdef DEBUG_FILE_PRINTDATA
            std::cout << temp << std::endl;
        #endif

        // res.push_back(temp);
        res->push_back(std::string(temp.c_str()));
    }

    return true;
}
