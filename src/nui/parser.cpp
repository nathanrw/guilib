#include <nui/parser.hpp>
#include <nui/ast.hpp>
#include <nui/assert.hpp>
#include <cctype>

nui::parser::parser()
{
}

nui::parser::~parser()
{
}

nui::status nui::parser::parse(const utf8_string& text, ast& out)
{
    // file := [entry]...
    // entry := <prop>|<widget>
    // prop := <name>: <value>
    // widget := <type> [name] {
    //   [entry]...
    // }
    // type := [a-zA-Z0-9_]+
    // name := [a-zA-Z0-9_]+
    // value := .*

    // Initialise the parser state
    parse_status status = {};
    status.cursor = text.c_str();
    status = next_token(status);
    if (!status.status) {
        return status.status;
    }

    // Read each entry
    std::vector<ast> entries;
    while (status.token.type != token::eof) {
        ast entry;
        status = parse_entry(status, entry);
        if (!status.status) {
            return status.status;
        }
        entries.push_back(entry);
    }

    // make a root node for the entries.
    out = ast::make_root(entries);
    
    return nui::status::OK;
}

nui::parser::parse_status nui::parser::parse_entry(
    parse_status status,
    ast& out
)
{
    // The first thing in an entry will be the widget type or property name.
    utf8_string name_or_type;
    status = parse_name(status, name_or_type);
    if (!status.status) {
        return status;
    }

    // For a property, the next sybol will be ':'.  Otherwise we are dealing
    // with a widget.
    if (status.token.type == token::colon) {
        status = parse_property(status, name_or_type, out);
    } else {
        status = parse_widget(status, name_or_type, out);
    }

    return status;
}

nui::parser::parse_status nui::parser::parse_widget(
    parse_status status,
    const utf8_string& type,
    ast& out
)
{
    utf8_string name;
    if (status.token.type != token::openbrace) {
        status = parse_name(status, name);
        if (!status.status) {
            return status;
        }
    }

    return parse_widget_contents(status, type, name, out);
}

nui::parser::parse_status nui::parser::parse_widget_contents(
    parse_status status,
    const utf8_string& type,
    const utf8_string& name,
    ast& out
)
{
    // Advance past the open brace.
    if (status.token.type != token::openbrace) {
		status.status = nui::status(NUI_ERROR_GENERIC, "Missing widget body: expecting {" );
        return status;
    }
    status = next_token(status);

    // Read the widget children.
    std::vector<ast> entries;
    while (status.token.type != token::eof) {

        // Look for the closing brace.
        if (status.token.type == token::closebrace) {
            status = next_token(status);
            if (!status.status) {
                return status;
            }
            break;
        }

        // Ok then there should be an entry.
        ast entry;
        status = parse_entry(status, entry);
        entries.push_back(entry);
    }

    // Assemble the widget.
    out = ast::make_widget(type, name, entries);

    return status;
}

nui::parser::parse_status nui::parser::parse_property(
    parse_status status,
    const utf8_string& name,
    ast& out
)
{
    // We've already parsed the name so check for the colon.
    if (status.token.type != token::colon) {
        status.status = nui::status(NUI_ERROR_GENERIC, "Expected :");
        return status;
    }
    status = next_token(status);
    if (!status.status) {
        return status;
    }

    // Parse the value.
    utf8_string value;
    status = parse_value(status, value);
    if (!status.status) {
        return status;
    }

    out = ast::make_property(name, value);

    return status;
}

nui::parser::parse_status nui::parser::parse_name(
    parse_status status,
    utf8_string& out
)
{
    if (status.token.type == token::symbol) {
        out = status.token.text;
        status = next_token(status);
        return status;
    }

    status.status = nui::status(NUI_ERROR_GENERIC, "Expected a symbol.");
    return status;
}

nui::parser::parse_status nui::parser::parse_value(
    parse_status status,
    utf8_string& out
)
{
    if (status.token.type == token::symbol ||
        status.token.type == token::value) {
        out = status.token.text;
        status = next_token(status);
        return status;
    }

    status.status = nui::status(NUI_ERROR_GENERIC, "Expected a symbol or value.");
    return status;
}

nui::parser::parse_status nui::parser::next_token(parse_status status)
{
    nui_utf8 text = status.cursor;
	NUI_ASSERT(text, "Text should not be null.");


    // Note: you can safely search UTF-8 encoded text for ASCII characters since
    // they are valid UTF-8 and will never be part of a multibyte sequence. As
    // such, I *believe* the following code to be correct.

    // Skip leading whitespace
    while (*text == ' ' || *text == '\r' || *text == '\n' || *text == '\r') {
        ++text;
    }
    if (!*text) {
        status.cursor = text;
        status.token = { nui::parser::token::eof, "" };
        return status;
    }

    // Skip comments
    if (*text == '/' && *(text+1) == '/') {
        while (*text && *text != '\n' && *text != '\r') {
            ++text;
        }
        if (*text == '\r') {
          ++text;
        }
        if (*text == '\n') {
            ++text;
        }
    }
    if (!*text) {
        status.cursor = text;
        status.token = { nui::parser::token::eof, "" };
        return status;
    }

    // Try to extract a token.

    if (*text == '"') {
        // string
        nui_utf8 text_end = text+1;
        while (*text_end && *text_end != '"') {
            ++text_end;
        }
        size_t len = text_end - text;
        status.cursor = text_end;
        status.token = { nui::parser::token::value,
                         utf8_string(text, len) };
        return status;

    } else if (*text == '{') {
        // open brace
        ++text;
        status.cursor = text;
        status.token = { nui::parser::token::openbrace, "{"};
        return status;

    } else if (*text == '}') {
        // close brace
        ++text;
        status.cursor = text;
        status.token = { nui::parser::token::closebrace, "}"};
        return status;

    } else if (*text == ':') {
        // colon
        ++text;
        status.cursor = text;
        status.token = { nui::parser::token::colon, ":"};
        return status;

    } else {
        // symbol
        nui_utf8 text_end = text;
        while (*text_end && (isalnum(*text_end) ||
                             *text_end == '_' ||
                             *text_end == '.')) {
            ++text_end;
        }
        if (text_end != text) {
            size_t len = text_end - text;
            status.cursor = text_end;
            status.token = { nui::parser::token::symbol,
                             utf8_string(text, len) };
            return status;
        }
    }

    status.status = nui::status(NUI_ERROR_GENERIC, "Bad token");
    
    return status;
}
