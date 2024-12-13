#include "util/public/log.h"
#include <iostream>

namespace XD::Log
{
    void log(const std::string_view& info)
    {
        std::cout << "info:    " << info << std::endl;
    }

    void logError(const std::string_view& info)
    {
        std::cerr << "error:   " << info << std::endl;
    }

    void logWarning(const std::string_view& info)
    {
        std::cout << "warning: " << info << std::endl;
    }
}