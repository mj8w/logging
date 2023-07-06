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
    std::cout << types.name(LogTypes::WARN) << std::endl;
    assert(std::string("WARN") == types.name(LogTypes::WARN));

    // Logger logger;

    LOG_INFO("First log");
    LOG_INFO.hex("LogTypes", &types, sizeof(types));

    LOG_INFO("Start Test");
    SteadyTime start_time = std::chrono::steady_clock::now();
    {
        std::stringstream output;
        Logger logger(output);

        for (int x = 0; x < 1000; x++) {
            LOG_INFO("Entry");
        }
    }
    SteadyTime end_time = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = end_time - start_time;
    long whole = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
    long fract = std::chrono::duration_cast<std::chrono::microseconds>(diff - std::chrono::seconds(whole)).count();
    LOG_INFO("Test Duration {:d}.{:06d} seconds", whole, fract);

    return 0;
}
