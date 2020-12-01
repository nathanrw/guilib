#ifndef NUI_UTF8_HPP
#define NUI_UTF8_HPP

#include <nui/types.h>
#include <istream>
#include <string>

namespace nui {
    using utf8_string = std::string;
    class text_reader;
    class status;
    class encoding;
    class wcs;
    namespace encodings {
        class utf8;
        class mbcs;
        class safe_ascii;
    }
}


/// A string encoding.  An implementation knows how to convert
/// from the encoding to utf-8.
///
/// String encodings are the stuff of nightmares.
class nui::encoding {
public:
    encoding();
    encoding(const encoding& that) =delete;
    encoding& operator=(const encoding& that) =delete;
    virtual ~encoding();

    /// Convert a string in this encoding into utf-8.
    ///
    /// This operation can fail, for instance if the given bytes are not
    /// actually a valid string in this encoding.
    ///
    /// The output string will ALWAYS be a valid utf-8 string. The
    /// implementation will decide how to achieve this, for instance by writing
    /// some default string, or by replacing unrepresentable sections with
    /// something else.
    ///
    /// If the returned status is `ok()` then the string was converted exactly.
    virtual status to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const =0;

    /// Convert a string in utf-8 to a string in this encoding.
    ///
    /// This operation might fail, for instance if the input string is not
    /// valid utf-8, or if the input string is not representible in this
    /// encoding, or if the given buffer is too small.
    ///
    /// The output string will ALWAYS be a valid string in this encoding. The
    /// implementation will decide how to achieve this, for instance by writing
    /// some default string, or by replacing unrepresentable characters with
    /// something else.
    ///
    /// If the returned status is `ok()` then the string was converted exactly.
    virtual status from_utf8(const utf8_string& in, std::string& out) const =0;
};


/// The UTF-8 encoding.
class nui::encodings::utf8 : public nui::encoding {
public:
    utf8();
    utf8(const utf8& that) =delete;
    utf8& operator=(const utf8& that) =delete;
    virtual ~utf8();

    virtual status to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const override;

    virtual status from_utf8(const utf8_string& in, std::string& out) const override;
};


/// Utilities for working with wide strings.
class nui::wcs {
public:
    wcs() = delete;
    wcs(const wcs& that) = delete;
    wcs& operator=(const wcs& that) = delete;
    ~wcs() = delete;

    static std::wstring from_utf8(nui_utf8 utf8);
    static std::wstring from_utf8(const utf8_string& utf8);
    static utf8_string to_utf8(const std::wstring& wstr);
    static utf8_string to_utf8(const wchar_t* wstr);
};


/// The 'safe' ASCII encoding. This is the subset of valid 'c strings' that are
/// also valid utf-8.
class nui::encodings::safe_ascii : public nui::encoding {
public:
    safe_ascii();
    safe_ascii(const safe_ascii& that) =delete;
    safe_ascii& operator=(const safe_ascii& that) =delete;
    virtual ~safe_ascii();

    virtual status to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const override;

    virtual status from_utf8(const utf8_string& in, std::string& out) const override;
};


/// The system mbcs encoding. Avoid!
class nui::encodings::mbcs : public nui::encoding {
public:
    mbcs();
    mbcs(const mbcs& that) =delete;
    mbcs& operator=(const mbcs& that) =delete;
    virtual ~mbcs();

    virtual status to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const override;

    virtual status from_utf8(const utf8_string& in, std::string& out) const override;
};


/// Reads text from an external source into a utf8-encoded string.
///
class nui::text_reader {
public:

    text_reader();
    text_reader(const text_reader& that)= delete;
    text_reader& operator=(const text_reader& that) = delete;
    ~text_reader();

    /// Read from a stream in the given encoding. This will keep reading from
    /// the stream until the stream can be read no more before doing the
    /// conversion.
    status read(
        const encoding& encoding,
        std::istream& stream,
        utf8_string& out
    ) const;
};

#endif
