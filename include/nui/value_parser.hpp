#ifndef NUI_VALUE_PARSER_H
#define NUI_VALUE_PARSER_H

#include <nui/types.h>
#include <nui/utf8.hpp>

namespace nui {
    class value_parser;
}

/// @brief Class to do tricky number/string conversions
///
class nui::value_parser {
public:
    value_parser() = delete;
    value_parser(const value_parser& that) = delete;
    value_parser& operator=(const value_parser& that) = delete;
    ~value_parser() = delete;

    /// @brief Attempt to interpret a string as a number
    /// @param input A string
    /// @param out A number, hopefully.
    /// @return Whether the parse was successful.
    /// 
    /// If this function returns an error status, 'out' is undefined. If this
    /// returns an OK status, 'out' will be a real number.
    /// 
    static status parse_real(const utf8_string& input, nui_real& out);

    /// @brief Interpret a string as a boolean value
    /// @param input A string
    /// @return The boolean value of the string
    /// 
    /// A string is false if it is the string "false" or if it is the empty
    /// string. A string is true otherwise.
    /// 
    static bool eval_bool(const utf8_string& input);

    /// @brief Convert an integer to a string
    /// @param value An integer
    /// @return The string representation thereof
    /// 
    static utf8_string to_string(nui_int value);

    /// @brief Convert a real number to a string
    /// @param value A number
    /// @return The string representation there of
    /// 
    /// The returned string will be parsable by parse_real().
    /// 
    static utf8_string to_string(nui_real value);
};

#endif
