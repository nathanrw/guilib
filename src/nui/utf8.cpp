#include <nui/utf8.hpp>
#include <nui/status.hpp>
#include <locale>
#include <codecvt>

// encoding

nui::encoding::encoding() = default;

nui::encoding::~encoding() = default;

// converter

std::wstring nui::converter::wstr_from_utf8(nui_utf8 utf8)
{
    // Note: codecvt is deprecated
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.from_bytes(utf8);
}

std::wstring nui::converter::wstr_from_utf8(const utf8_string & utf8)
{
    // Note: codecvt is deprecated
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.from_bytes(utf8);
}

nui::utf8_string nui::converter::wstr_to_utf8(const std::wstring & wstr)
{
    // Note: codecvt is deprecated
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.to_bytes(wstr);
}

nui::utf8_string nui::converter::wstr_to_utf8(const wchar_t* wstr)
{
    // Note: codecvt is deprecated
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.to_bytes(wstr);
}

nui::status nui::converter::bstr_to_utf8(const char* bytes, size_t num_bytes, const encoding& encoding, utf8_string& out)
{
    return encoding.to_utf8(bytes, num_bytes, out);
}

nui::status nui::converter::bstr_to_utf8(const std::string& bytes, const encoding& encoding, utf8_string& out)
{
    return encoding.to_utf8(bytes.c_str(), bytes.size() + 1, out);
}

nui::status nui::converter::bstr_from_utf8(const utf8_string& in, const encoding& encoding, std::string& out)
{
    return encoding.from_utf8(in, out);
}

// utf8

nui::encodings::utf8::utf8() = default;

nui::encodings::utf8::~utf8() = default;

nui::status nui::encodings::utf8::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
{
    out = utf8_string(bytes, num_bytes);

    return status::OK;
}

nui::status nui::encodings::utf8::from_utf8(const utf8_string& in, std::string& out) const
{
    out = in;

    return status::OK;
}

// safe_ascii

nui::encodings::safe_ascii::safe_ascii() = default;

nui::encodings::safe_ascii::~safe_ascii() = default;

nui::status nui::encodings::safe_ascii::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
{
    out = utf8_string(bytes, num_bytes);

    return status::OK;
}

nui::status nui::encodings::safe_ascii::from_utf8(const utf8_string& in, std::string& out) const
{
    out = in;

    return status::OK;
}

// mbcs

nui::encodings::mbcs::mbcs() = default;

nui::encodings::mbcs::~mbcs() = default;

nui::status nui::encodings::mbcs::to_utf8(const char* bytes, size_t num_bytes, utf8_string& out) const
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
    wstr_storage.resize(required_size+1);
    size_t converted = mbstowcs(wstr_storage.data(), bytes, required_size+1);
    if (converted == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "mbcstowcs failed.");
    }

    out = converter::wstr_to_utf8(wstr_storage.data());
    return status::OK;
}

nui::status nui::encodings::mbcs::from_utf8(const utf8_string& in, std::string& out) const
{
    // potential optimisation: no-op when mbcs encoding == utf8

    std::wstring wstr = converter::wstr_from_utf8(in);

    size_t required_size = wcstombs(0, wstr.c_str(), 0);
    if (required_size == (size_t)-1) {
        return status(NUI_ERROR_ENCODING, "Input string is not a valid wcs.");
    }

    // ignoring problem cases
    //  - required_size needs too much memory
    //  - required_size is max size_t -1 (i.e. required_size+1 == 0)
    std::vector<char> str_storage;
    str_storage.resize(required_size+1);
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

nui::status nui::text_reader::read(
    const encoding& encoding,
    std::istream& stream,
    utf8_string& out
) const
{
    std::string s(std::istreambuf_iterator<char>(stream), {});
    return converter::bstr_to_utf8(s, encoding, out);
}
