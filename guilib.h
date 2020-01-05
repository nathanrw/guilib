// A design for a simple GUI library

// Goals:
//   - simple and *narrow* API for ease of binding generation
//   - 'retained mode', no weird stuff like source-line based
//     widget identifiers
//   - renderer agnostic
//   - abstract support for sounds (inject, or return...)
//   - renderer agnostic - but try to do as much work as possible

struct nui_gui;
struct nui_data;

typedef uint32_t nui_unsigned;
typedef int32_t nui_int;
typedef uint32_t nui_error;
typedef const char* nui_utf8;
typedef uint64_t nui_size;
typedef float nui_real;

// Widget identifier. 0 always identifies the root widget, which always exists and cannot be destroyed.
typedef uint32_t nui_widget_id;

enum nui_widget_type {
    NUI_WIDGET_TYPE_WINDOW = 0,
    NUI_WIDGET_TYPE_BUTTON
};

enum nui_error {
    NUI_ERROR_OK = 0,
    NUI_ERROR_UNKNOWN
}


//----- Data node interface

struct nui_data;
typedef enum nui_error nui_data_get_child_node_by_name_func(
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
enum nui_error nui_create_widgets_from_file(
    struct nui_gui*, 
    nui_widget_id parent, 
    nui_utf8 filename
);

// Create widgets from text
enum nui_error nui_create_widgets_from_text(
    struct nui_gui*, 
    nui_widget_id parent, 
    nui_utf8 text
);

// Create a widget.
//
// If a widget with the same name already exists, the name is disambiguated with a number.
//
// 'parent' must be a widget with a 'client area'.  The new widget will be displayed in
// the 'client area' of the parent widget.
//
// the newly created widget may have any number of automatically created child widgets in 
// its 'non client area' - e.g. window frame buttons.
enum nui_error nui_create_widget(
    struct nui_gui*, 
    enum nui_widget_type type, 
    nui_utf8 name, 
    nui_widget_id parent, 
    nui_widget_id* widget_id_out
);

// Get a list of child widgets.
enum nui_error nui_get_widget_children(
    struct nui_gui*, 
    enum nui_widget_id, 
    enum nui_widget_id* buf, 
    nui_size buf_size, 
    nui_size* size_needed_out
);

// Get a named widget.  The name can use 'dot' notation to navigate
// the hierarchy.
enum nui_error nui_get_widget(
    struct nui_gui*,
    nui_widget_id parent,
    nui_utf8 name,
    nui_widget_id* widget_id_out
);

// Get/set properties.
enum nui_error nui_get_widget_properties(
    struct nui_gui*, 
    nui_widget_id widget, 
    struct nui_data* out
);


//----- Input

// Note: need to flesh this out.  Need to pass input from the application into the library,
// and return any requested actions.  Need to support arbitrary input devices.
enum nui_error nui_input(struct nui_gui*);

struct nui_command {
    nui_widget_id widget;
};

//----- Update

// The UI doesn't have its own main loop, so the application must inform it of 
// the passage of time.  This is used to update animations and so on.
enum nui_error nui_update(struct nui_gui*, nui_real dt);

// how to associate a widget with a value in the program?

// label foo {
//   expr: "bar.qux"
// }

// how would the above expr be evaluated?   inject an interface?   some api like:

struct nui_compiled_expr;

// expr factory func
typedef enum nui_error nui_compile_expr_func(nui_utf8 expr, struct nui_compiled_expr* out);

// expr class
typedef enum nui_error evaluate_expr_func(
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

// This needs to output either a sequence of drawing commands as per nuklear, or something more abstract
// to allow for greater customisation of what is displayed by the application
enum nui_error nui_draw(struct nui_gui*);
