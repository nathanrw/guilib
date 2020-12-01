#ifndef NUI_PROPERTY_CURSOR_HPP
#define NUI_PROPERTY_CURSOR_HPP

#include <nui/types.h>
#include <nui/status.hpp>


namespace nui {
    class property_cursor;
}


/// A property cursor is an interface to arbitrary data.
class nui::property_cursor {
public:


    /// Default constructor
    property_cursor();

    /// Copying is prohibited
    property_cursor(const property_cursor&) =delete;

    /// Assignment is prohibited
    property_cursor& operator=(const property_cursor&) =delete;

    /// Destructor
    virtual ~property_cursor();


    /// Get the name of the property at the cursor.
    virtual const utf8_string& name() const =0;

    /// Get the value of the property in various forms.  These will not always make
    /// sense for all properties, and might return garbage or default values depending
    /// on the property type.

    /// Get the value as an int, or 0 if this is not meaningful.
    virtual nui_int get_as_int() const =0;

    /// Get the value as a real number, or 0 if this is not meaniningful.
    virtual nui_real get_as_real() const =0;

    /// Get the value as a string, or "" if this is not meaningful.
    virtual const utf8_string& get_as_string() const =0;

    /// Get the value as a boolean, or 'false' if this is not meaningful.
    virtual bool get_as_bool() const =0;

    /// Get the value as a buffer. If this is not meaningful the returned status
    /// will be a failure.
    ///
    /// 'buffer_size' should be the capacity of the buffer. If this is too small
    /// to fit the data, the call will fail, but 'size_needed' will contain the
    /// minimum size to which the buffer should be resized to ensure success. On
    /// success, 'size_needed' will contain the actual size of the data returned.
    ///
    /// Implementations should conform the the following convention:
    ///
    ///     int                        nui_int
    ///     real                       nui_real
    ///     string                     null-terminated utf-8 c string
    ///     bool                       nui_int
    ///     array of int or real       sequence of nui_int or nui_real
    ///
    /// A property node that has a sequence of children of the same primitive
    /// type should act like an array here if possible.
    ///
    /// But otherwise the returned data is a binary blob, it's up for the
    /// implementation to decide & the caller to understand.
    ///
    virtual status get_as_data(nui_byte* buffer,
                               size_t buffer_size,
                               size_t& size_needed) =0;

    /// Get the number of child properties.
    virtual size_t number_of_children() const =0;

    /// Traverse the tree of properties.
    virtual status select_next_sibling() =0;
    virtual status select_parent() =0;
    virtual status select_child(nui_int index) =0;
    virtual status select_named_child(const utf8_string& name) =0;


    /// Setters. Set the value of the property in various ways.


    /// Set the value as an int. The call will fail if this makes no sense.
    virtual status set_as_int(nui_int value) =0;

    /// Set the value as a real. The call will fail if this makes no sense.
    virtual status set_as_real(nui_real value) =0;

    /// Set the value as a string. The call will fail if this makes no sense.
    ///
    /// In particular, set_as_string() on a numerical property with a numerical
    /// string should work.
    virtual status set_as_string(const utf8_string& value) =0;

    /// Set the value as a bool. The call will fail if this makes no sense.
    virtual status set_as_bool(bool value) =0;

    /// Offer up a buffer for the property to set its data from.  This will be
    /// interpreted according to the type of the property, and the behaviour
    /// should have symmetry with get_as_data().
    virtual status set_as_data(const nui_byte* buffer, size_t buffer_size) =0;


private:
};


#endif
