#include <nui/status.hpp>

nui::status::status()
  : m_error_code(NUI_ERROR_OK),
    m_error_message("")
{
}

nui::status::status(nui::status::EOK)
  : m_error_code(NUI_ERROR_OK),
    m_error_message("")
{
}

nui::status::status(nui_error_code code, const utf8_string& message)
  : m_error_code(code),
    m_error_message(message)
{
}

nui::status::status(const status& that) = default;

nui::status& nui::status::operator=(const status& that) = default;

nui::status::~status() = default;

nui::status::operator bool() const
{
    return m_error_code == NUI_ERROR_OK;
}

bool nui::status::ok() const
{
    return !!*this;
}

nui_error_code nui::status::error_code() const
{
    return m_error_code;
}

const nui::utf8_string& nui::status::error_message() const
{
    return m_error_message;
}