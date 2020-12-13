#include <nui/assert.hpp>

#include <sstream>

void nui::handle_assertion(
    const char* expr,
    const char* filename,
    const char* function,
    int line,
    const char* msg
)
{
    std::stringstream buf;
    buf << "Assertion failed: " << msg << std::endl
        << "Expression: " << expr << std::endl
        << "File: " << filename << std::endl
        << "Function: " << function << std::endl
        << "Line: " << line << std::endl;

    throw std::runtime_error(buf.str());
}
