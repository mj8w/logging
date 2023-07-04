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

#define LOG_INFO m_logger.log(LogTypes::INFO, __FILE__, __LINE__)
#define LOG_WARN m_logger.log(LogTypes::WARN, __FILE__, __LINE__)
#define LOG_ERR m_logger.log(LogTypes::ERR, __FILE__, __LINE__)
#define LOG_FAIL m_logger.log(LogTypes::FAIL, __FILE__, __LINE__)

class Logger {
private:
    std::stringstream 	m_stream;
    std::ostream& 		m_outputStream;
    LogTypes			m_types;
    LogTypes::Types		m_type;
    std::string			m_pos;		// filename and line #

public:
    Logger(std::ostream& outputStream = std::cout) :
    	m_stream(),
		m_outputStream(outputStream),
		m_types(),
		m_type(LogTypes::INFO),
		m_pos(""){}

    Logger& log(const LogTypes::Types type, const std::string& filePath, const uint32_t linenum) {
    	m_type = type;
        size_t lastSep = filePath.find_last_of("/\\");
        if (lastSep != std::string::npos) {
        	m_pos = fmt::format("{}#{}", filePath.substr(lastSep + 1), linenum);
        } else {
        	m_pos = fmt::format("{}#{}", filePath, linenum);
        }
    	return *this;
    }

    Logger& operator<<(const std::string& str) {
        m_stream << str;
        return *this;
    }

    template <typename... Args>
    void operator()(const std::string& formatString, const Args&... args) {
        m_stream << fmt::format(formatString, args...);
    }

    Logger& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if (manipulator == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
            m_outputStream << m_stream.str() << std::endl;
            m_stream.str("");
        }
        return *this;
    }
};
