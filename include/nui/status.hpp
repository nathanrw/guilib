#ifndef NUI_STATUS_HPP
#define NUI_STATUS_HPP

#include <nui/types.h>
#include <nui/utf8.hpp>
#include <string>
#include <vector>

namespace nui {
    class status;
}


/// The result of an operation, wrapping up an error code and
/// an error message.
class nui::status {
public:


    /// The default status is OK
    status();

    /// Construct a status from an error code and message.
    status(nui_error_code code, const utf8_string& message);

    // Copy.
    status(const status& that);

    // Assign.
    status& operator=(const status& that);

    // Destructor.
    ~status();


    /// Returns ok()
    operator bool() const;

    /// Returns true for an OK status, false otherwise.
    bool ok() const;

    /// Get the error code.
    nui_error_code error_code() const;

    /// Get the error message.
    const utf8_string& error_message() const;


private:
    nui_error_code m_error_code;
    utf8_string m_error_message;
};


#endif
