// A design for a simple GUI library

// Goals:
//   - simple and *narrow* API for ease of binding generation
//   - 'retained mode', no weird stuff like source-line based
//     widget identifiers
//   - renderer agnostic
//   - abstract support for sounds (inject, or return...)
//   - renderer agnostic - but try to do as much work as possible

struct ctx {};

typedef uint32_t error;
typedef const char* utf8_string;

// Widget identifier. 0 always identifies the root widget, which always exists and cannot be destroyed.
typedef uint32_t widget_id;

typedef uint32_t action_id;

enum widget_type {
    window = 0,
    button = 1
};


//----- Create and configure widgets

// When a widget is created, it is referencable by name or id, but is not visible - must
// set the 'visible' property to TRUE.

// Create widgets from text from a file
error create_widgets_from_file(struct ctx*, widget_id parent, utf8_string filename);

// Create widgets from text
error create_widgets_from_text(struct ctx*, widget_id parent, utf8_string text);

// Create a widget.
//
// If a widget with the same name already exists, the name is disambiguated with a number.
//
// 'parent' must be a widget with a 'client area'.  The new widget will be displayed in
// the 'client area' of the parent widget.
//
// the newly created widget may have any number of automatically created child widgets in 
// its 'non client area' - e.g. window frame buttons.
error create_widget(
  struct ctx*, 
  widget_type type, 
  utf8_string name, 
  widget_id parent, 
  widget_id* widget_id_out
);

// Get a list of child widgets.
error get_children(struct ctx*, widget_id, widget_id* buf, uint64_t buf_size, uint64_t* size_needed_out);

// Get/set properties.
error get_properties(struct ctx*, widget_id widget, struct data_node* out);


//----- Input

// Note: need to flesh this out.  Need to pass input from the application into the library,
// and return any requested actions.  Need to support arbitrary input devices.
error input(struct ctx*);

struct command {
    widget_id widget;
    action_id action; // register action ids via api? ... or just ask widget for its action code?
};


//----- Update

// The UI doesn't have its own main loop, so the application must inform it of 
// the passage of time.  This is used to update animations and so on.
error update(float dt);

// how to associate a widget with a value in the program?

// label foo {
//   expr: "bar.qux"
// }

// how would the above expr be evaluated?   inject an interface?   some api like:


struct data_node;
struct compiled_expr;

// expr factory func
typedef error compile_expr_func(utf8_string expr, struct compiled_expr* expr);

// expr class
typedef error evaluate_expr_func(struct compiled_expr* expr, struct data_node* out); // called when updating widget
typedef error free_compiled_expr_func(struct compiled_expr*);
struct compiled_expr {
    evaluate_expr_func evaluate;
    free_compiled_expr_func free;
    void* handle;
};

// data node class
typedef error get_child_node_by_name(struct data_node* node, utf8_string key, struct data_node* out);
// get_child_node_by_index
// get_value_string
// get_value_int
// get_value_real
// set_value_string
// set_value_int
// set_value_real
// get_num_children
// free_data_node
struct data_node {
  // ...    
  void* handle; 
};


//----- Draw

// This needs to output either a sequence of drawing commands as per nuklear, or something more abstract
// to allow for greater customisation of what is displayed by the application
error draw(struct ctx*);
