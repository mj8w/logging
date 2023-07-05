//============================================================================
// Name        : logging.h
// Author      : Micheal Wilson
// Copyright   : Copyright 2023
// Description : Logging Classes
//============================================================================

#pragma once

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <chrono>

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

using SteadyTime = std::chrono::time_point<std::chrono::steady_clock>;

class Logger {
private:
    std::stringstream 	m_stream;   // assemble logs here
    std::ostream& 		m_output;   // output to here when the whole log has been created
    LogTypes			m_types;    // helper for using types
    LogTypes::Types		m_type;     // the type of this log
    std::string			m_pos;		// filename and line #
    bool                m_use_time; // use this field?
    bool                m_use_type;
    bool                m_use_pos;
    SteadyTime          m_start_time;    // when the object was created

public:
    Logger(std::ostream& outputStream = std::cout) :
    	m_stream(),
		m_output(outputStream),
		m_types(),
		m_type(LogTypes::INFO),
		m_pos(""),
        m_use_time(true),
        m_use_type(true),
        m_use_pos(true),
        m_start_time(std::chrono::steady_clock::now())
        {}

    // allow caller to filter what fields are shown in the output
    void fields(const bool time, const bool type, const bool pos) {
        m_use_time = time;
        m_use_type = type;
        m_use_pos = pos;
    }

    // to be used with macros above, this sets the context for the log
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

    // std::string passed to << operator
    Logger& operator<<(const std::string& str) {
        m_stream << str;
        return *this;
    }

    Logger& operator<<(const char ch) {
        m_stream << ch;
        return *this;
    }

    // functor allows passing the fmt library operands to the log
    template <typename... Args>
    void operator()(const std::string& formatString, const Args&... args) {
        *this << fmt::format(formatString, args...) << std::endl;
    }

    // detect end of line (endl is a manipulator function)
    // and output the log when it is detected
    Logger& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if (manipulator == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
            if (m_use_time) {
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> diff = now - m_start_time;
                long whole = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
                long fract = std::chrono::duration_cast<std::chrono::duration<long, std::micro>>(diff).count() % 1000000;
                m_output << fmt::format("{:d}.{:04d} - ", whole, fract);
            }
            if (m_use_type) {
                m_output << m_types.name(m_type) << ": ";
            }
            m_output << m_stream.str();
            if (m_use_pos) {
                m_output << " | " << m_pos;
            }
            m_output << std::endl;
            m_stream.str("");
        }
        return *this;
    }

    // hexadecimal memory dump log
    template<typename T>
    Logger& hex(const std::string name, const T* ptr, const size_t size) {
        *this << fmt::format("{} is {} bytes @{:x}\n", name, size, reinterpret_cast<uintptr_t>(ptr));

        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(ptr);

        for (size_t i = 0; i < size; i++) {
            // Print the offset at the beginning of each line
            if (i % 16 == 0) {
                *this << fmt::format("{:04x}: ", i);
            }

            size_t index = i + 1;
            *this << fmt::format("{:02x}", static_cast<int>(bytes[i]));

            if (index % 4 == 0 && index % 16 != 0) {
                *this << " : ";
            } else {
                *this << ' ';
            }

            if (index % 16 == 0) {
                if (index == size) {
                    *this << std::endl;
                } else {
                    *this << '\n';
                }
            }
        }
        // Add a newline if the last line was not complete
        if (size % 16 != 0) {
            *this << std::endl;
        }
        return *this;
    }
};
