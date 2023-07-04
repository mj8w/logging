/*
 * logging.h
 *
 *  Created on: Jul 4, 2023
 *      Author: mail4
 */

#pragma once

#include <iostream>
#include <string>
#include <sstream>

#define FMT_HEADER_ONLY
#include "fmt/format.h"


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
