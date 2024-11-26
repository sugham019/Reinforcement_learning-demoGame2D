#include "ErrorHandler.hpp"
#include <iostream>

void reportError(const std::string& errorMsg){
    std::cerr << errorMsg << std::endl;
    std::terminate();
}

void reportWarning(const std::string& warningMsg){
    std::cerr << warningMsg << std::endl;
}