#include <nui/utf8.hpp>
#include <locale>
#include <codecvt>

// encoding

nui::encodings::encoding::encoding() = default;

nui::encodings::encoding::~encoding() = default;

// utf8

nui::encodings::utf8::utf8() = default;

nui::encodings::utf8::~utf8() = default;

status nui::encodings::utf8::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
{
    out = utf8_string(bytes, num_bytes);

    return status::OK;
}

status nui::encodings::utf8::from_utf8(const utf8_string& in, std::string& out) const
{
    out = in;

    return status::OK;
}

// safe_ascii

nui::encodings::safe_ascii::safe_ascii() = default;

nui::encodings::safe_ascii::~safe_ascii() = default;

status nui::encodings::safe_ascii::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
{
    out = utf8_string(bytes, num_bytes);

    return status::OK;
}

status nui::encodings::safe_ascii::from_utf8(const utf8_string& in, std::string& out) const
{
    out = in;

    return status::OK;
}

// mbcs

nui::encodings::mbcs::mbcs() = default;

nui::encodings::mbcs::~mbcs() = default;

status nui::encodings::mbcs::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
{
    // potential optimisation: no-op when mbcs encoding == utf8

    size_t required_size = mbstowcs(0, bytes, 0);
    if (required_size == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "Input string is not a valid mbcs.");
    }

    // ignoring problem cases
    //  - required_size needs too much memory
    //  - required_size is max size_t -1 (i.e. required_size+1 == 0)
    std::vector<wchar_t> wstr_storage;
    wstr_storage.expand(required_size+1);
    size_t converted = mbstowcs(wstr_storage.data(), bytes, required_size+1);
    if (converted == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "mbcstowcs failed.");
    }

    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    out = cvt.to_bytes(wstr_storage.data());
    return status::OK;
}

status nui::encodings::mbcs::from_utf8(const utf8_string& in, std::string& out) const
{
    // potential optimisation: no-op when mbcs encoding == utf8

    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    std::wstring wstr = cvt.from_bytes(in);

    size_t required_size = wcstombs(0, wstr.c_str(), 0);
    if (required_size == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "Input string is not a valid wcs.");
    }

    // ignoring problem cases
    //  - required_size needs too much memory
    //  - required_size is max size_t -1 (i.e. required_size+1 == 0)
    std::vector<char> str_storage;
    str_storage.expand(required_size+1);
    size_t converted = wcstombs(str_storage.data(), wstr.c_str(), required_size+1);
    if (converted == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "wcstombs failed.");
    }

    out = utf8_string(str_storage.data(), required_size);
    return status::OK;
}

// text_reader

nui::text_reader::text_reader() = default;

nui::text_reader::~text_reader() = default;

status nui::text_reader::read(
    const encoding& encoding,
    std::istream& stream,
    utf8_string& out
) const
{
    // TODO
}