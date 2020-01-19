// A design for a simple GUI library

// Goals:
//   - simple and *narrow* API for ease of binding generation
//   - 'retained mode', no weird stuff like source-line based
//     widget identifiers
//   - renderer agnostic
//   - abstract support for sounds (inject, or return...)
//   - renderer agnostic - but try to do as much work as possible

#ifndef NUI_
#define NUI_

#define NUI_API // TODO: symbols

#include "stdint.h"

struct nui_gui;
struct nui_data;

typedef uint32_t nui_unsigned;
typedef int32_t nui_int;
typedef uint32_t nui_error;
typedef const char* nui_utf8;
typedef uint64_t nui_size;
typedef float nui_real;

// Widget identifier. 0 always identifies the root widget, which always exists
// and cannot be destroyed.
typedef uint32_t nui_widget_id;

enum nui_widget_type {
    NUI_WIDGET_TYPE_WINDOW = 0,
    NUI_WIDGET_TYPE_BUTTON
};

enum nui_error_code {
  NUI_ERROR_OK = 0,
  NUI_ERROR_UNKNOWN
};



//----- Setup

NUI_API bool nui_create_gui(struct nui_gui** out);

NUI_API bool nui_destroy_gui(struct nui_gui* gui);

NUI_API enum nui_error_code nui_get_last_error_code(struct nui_gui* gui);

NUI_API nui_utf8 nui_get_last_error_message(struct nui_gui* gui);

NUI_API void nui_log_error(
    struct nui_gui* gui,
    enum nui_error_code code,
    nui_utf8 message
);



//----- Data node interface

struct nui_data;
typedef bool nui_data_get_child_node_by_name_func(
    struct nui_data* node, 
    nui_utf8 key,
    struct nui_data* out
);
// get_child_node_by_index
// get_value_string
// get_value_int
// get_value_real
// set_value_string
// set_value_int
// set_value_real
// get_num_children
// get_last_error
// log_error
// acquire_reference
// release_reference
struct nui_data {
  nui_data_get_child_node_by_name_func get_child_node_by_name;
  // ...
  void* handle; 
};



//----- Create and configure widgets

// When a widget is created, it is referencable by name or id, but is not visible - must
// set the 'visible' property to TRUE.

// Create widgets from text from a file
NUI_API enum bool nui_create_widgets_from_file(
    struct nui_gui*,
    nui_widget_id parent,
    nui_utf8 filename
);

// Create widgets from text
NUI_API enum bool nui_create_widgets_from_text(
    struct nui_gui*,
    nui_widget_id parent,
    nui_utf8 text
);

// Create a widget.
//
// If a widget with the same name already exists, the name is disambiguated
// with a number.
//
// 'parent' must be a widget with a 'client area'. The new widget will be
// displayed in the 'client area' of the parent widget.
//
// the newly created widget may have any number of automatically created child
// widgets in its 'non client area' - e.g. window frame buttons.
NUI_API bool nui_create_widget(
    struct nui_gui*,
    enum nui_widget_type type,
    nui_utf8 name,
    nui_widget_id parent,
    nui_widget_id* widget_id_out
);

// Get a list of child widgets.
NUI_API bool nui_get_widget_children(
    struct nui_gui*,
    enum nui_widget_id,
    enum nui_widget_id* buf,
    nui_size buf_size,
    nui_size* size_needed_out
);

// Get a named widget.  The name can use 'dot' notation to navigate
// the hierarchy.
NUI_API bool nui_get_widget(
    struct nui_gui*,
    nui_widget_id parent,
    nui_utf8 name,
    nui_widget_id* widget_id_out
);

// Get/set properties.
NUI_API bool nui_get_widget_properties(
    struct nui_gui*,
    nui_widget_id widget,
    struct nui_data* out
);


//----- Input

// Note: need to flesh this out.  Need to pass input from the application into the library,
// and return any requested actions.  Need to support arbitrary input devices.
NUI_API bool nui_input(struct nui_gui*);

struct nui_command {
    nui_widget_id widget;
};

//----- Update

// The UI doesn't have its own main loop, so the application must inform it of 
// the passage of time.  This is used to update animations and so on.
NUI_API bool nui_update(struct nui_gui*, nui_real dt);

// how to associate a widget with a value in the program?

// label foo {
//   expr: "bar.qux"
// }

struct nui_compiled_expr;

// expr factory func
typedef bool nui_compile_expr_func(
    struct nui_gui*,
    nui_utf8 expr,
    struct nui_compiled_expr* out
);

// expr class
typedef bool evaluate_expr_func(
    struct nui_gui*,
    struct nui_compiled_expr* expr, 
    struct nui_data* out
);
// acquire_reference
// release_reference
struct compiled_expr {
    evaluate_expr_func evaluate;
    // ...
    void* handle;
};



//----- Draw

// This needs to output either a sequence of drawing commands as per nuklear,
// or something more abstract to allow for greater customisation of what is
// displayed by the application
NUI_API bool nui_draw(struct nui_gui*);


// ****************************************************************************

#ifdef NUI_IMPLEMENT

#ifndef __cplusplus
#error NUI requires a C++ compiler.
#endif

namespace nui {

    class gui {
    public:
        gui();
        virtual ~gui();
    };

    class intrusive_refcount {
    };

    class data : public intrusive_refcount {
    public:
        data();
        virtual ~data();
    };

    class widget {
    public:
        widget();
        virtual ~widget();
    };

}

struct nui_gui {
    nui_error_code m_last_error_code;
    std::string m_last_error_message;
};

bool nui_create_gui(struct nui_gui** out)
{
    assert(out);
    *out = new nui_gui;
}

bool nui_destroy_gui(struct nui_gui* gui)
{
    assert(gui);
    delete gui;
}

enum nui_error_code nui_get_last_error_code(struct nui_gui* gui)
{
    assert(gui);

    return gui->m_gui.get_last_error_code();
}

nui_utf8 nui_get_last_error_message(struct nui_gui* gui)
{
    assert(gui);

    return gui->m_gui.get_last_error_message().get();
}

void nui_log_error(
    struct nui_gui* gui,
    enum nui_error_code code,
    nui_utf8 message
)
{
    assert(gui);

    gui->m_gui.log_error(code, message);
}

bool nui_create_widgets_from_file(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 filename
)
{
    assert(gui);

    // istream (filename)
    // create_widgets(istream)
}

bool nui_create_widgets_from_text(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 text
)
{
    assert(gui);

    // istream(text)
    // create_widgets(text)
}

bool nui_create_widget(
    struct nui_gui* gui,
    enum nui_widget_type type,
    nui_utf8 name,
    nui_widget_id parent,
    nui_widget_id* widget_id_out
)
{
    assert(gui);
    // ...
}

bool nui_get_widget_children(
    struct nui_gui* gui,
    enum nui_widget_id,
    enum nui_widget_id* buf,
    nui_size buf_size,
    nui_size* size_needed_out
)
{
    assert(gui);
    // ...
}

bool nui_get_widget(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 name,
    nui_widget_id* widget_id_out
)
{
    assert(gui);
    // ...
}

bool nui_get_widget_properties(
    struct nui_gui* gui,
    nui_widget_id widget,
    struct nui_data* out
)
{
    assert(gui);
    // ...
}

#endif // NUI_IMPLEMENT

#endif // NUI_
