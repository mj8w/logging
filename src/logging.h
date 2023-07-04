/*
 * logging.h
 *
 *  Created on: Jul 4, 2023
 *      Author: mail4
 */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <sstream>

// enable this for header only use of FMT library(slower compile)
//  - then you don't need the library in the build...
// #define FMT_HEADER_ONLY
#include "fmt/format.h"

struct LogTypes {
	enum Types {
		INFO,
		WARN,
		ERR,
		FAIL
	};

	std::string name(Types type) {
		return names.at(type);
	}

private:
	std::map<Types, std::string> names = {
			{INFO, "INFO"},
			{WARN, "WARN"},
			{ERR, "ERR"},
			{FAIL, "FAIL"},
	};
};

class Log {
private:
    std::stringstream m_stream;
    std::ostream& m_outputStream;

public:
    Log(std::ostream& outputStream = std::cout) : m_outputStream(outputStream) {}

    Log& operator<<(const std::string& str) {
        m_stream << str;
        return *this;
    }

    template <typename... Args>
    void operator()(const std::string& formatString, const Args&... args) {
        m_stream << fmt::format(formatString, args...);
    }

    Log& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if (manipulator == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
            m_outputStream << m_stream.str() << std::endl;
            m_stream.str("");
        }
        return *this;
    }
};
