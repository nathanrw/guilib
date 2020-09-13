#ifndef NUI_PARSER_HPP
#define NUI_PARSER_HPP

#include <nui/types.h>
#include <nui/status.hpp>
#include <nui/utf8.hpp">

namespace nui {
    class parser;
    class ast;
    class status;
}


/// Parser for the widget definition language.  This is a simple
/// language that looks like so:
///
///      file := [entry]...
///      entry := <prop>|<widget>
///      prop := <name>: <value>
///      widget := <type> [name] {
///        [entry]...
///      }
///      type := [a-zA-Z0-9_]+
///      name := [a-zA-Z0-9_]+
///      value := .*
///
/// In a file, you would have something like
///
///      button my_button {
///        height: 30
///        label {
///          text: "Hello"
///        }
///      }
///
class nui::parser {
public:


    // Ctor.
    parser();

    // Copying is prohibited.
    parser(const parser& that) = delete;

    // Assignment is prohibited.
    parser& operator=(const parser& that) = delete;

    // Destructor.
    ~parser();


    /// Parse an AST from the given text.
    status parse(const utf8_string& text, ast& out) ;


private:

    struct token {
        enum type {
            symbol,
            colon,
            openbrace,
            closebrace,
            value,
            eof
        };
        type type;
        utf8_string value;
    };

    struct parse_status {
        status status;
        token token;
        nui_utf8 cursor;
    };

    parse_status parse_entry(
        parse_status status,
        ast& out
    );

    parse_status parse_widget(
        parse_status status,
        const utf8_string& type,
        ast& out
    );

    parse_status parse_widget_contents(
        parse_status status,
        const utf8_string& type,
        const utf8_string& name,
        ast& out
    );

    parse_status parse_property(
        parse_status status,
        const utf8_string& name,
        ast& out
    );

    parse_status parse_name(
        parse_status status,
        utf8_string& out
    );

    parse_status parse_value(
        parse_status status,
        utf8_string& out
    );

    parse_status next_token(parse_status status);


};


#endif
