//============================================================================
// Name        : main.cpp
// Author      : Micheal Wilson
// Copyright   : Copyright 2023
// Description : Logging Example Code
//============================================================================

#include <iostream>
#include "logging.h"
#include <assert.h>

int main() {
    // test format library
    std::cout << fmt::format("format example {}", 42) << std::endl; // prints Logging example
    assert(std::string("format example 42") == fmt::format("format example {}", 42));

    // test LogTypes object
    LogTypes types;
    std::cout << types.name(LogTypes::WARN);
    assert(std::string("WARN") == types.name(LogTypes::WARN));

    Logger m_logger;

    return 0;
}
