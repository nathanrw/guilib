#include <nui/value_parser.hpp>
#include <nui/status.hpp>
#include <string>

nui::status nui::value_parser::parse_real(const utf8_string& input, nui_real& out)
{
    // note: locale dependent. Probably don't want it to be.
    try {
        size_t end_idx = 0;
        out = std::stod(input, &end_idx);
        if (end_idx != input.size()) {
            return status(NUI_ERROR_FAILED, "Part of string not used in parse");
        }
    } catch (const std::invalid_argument& e) {
        return status(NUI_ERROR_FAILED, e.what());
    } catch (const std::out_of_range& e) {
        return status(NUI_ERROR_FAILED, e.what());
    }
    return status::OK;
}

bool nui::value_parser::eval_bool(const utf8_string& input)
{
    const utf8_string false_str = "false";
    return !input.empty() && input != false_str;
}

nui::utf8_string nui::value_parser::to_string(nui_int value)
{
    return std::to_string(value);
}

nui::utf8_string nui::value_parser::to_string(nui_real value)
{
    // Note: locale dependent with decimal separator?
    return std::to_string(value);
}
