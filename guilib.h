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
error create_widget(struct ctx*, utf8_string name, widget_id parent, widget_id* widget_id_out);

// Get a list of child widgets.
error get_children(struct ctx*, widget_id, widget_id* buf, uint64_t buf_size, uint64_t* size_needed_out);

// Get/set properties.
error set_property_string(struct ctx*, widget_id widget, utf8_string key, utf8_string value);
error set_property_real(struct ctx*, widget_id widget, utf8_string key, float value);
error set_property_int(struct ctx*, widget_id widget, utf8_string key, uint32_t value);
error get_property_string(struct ctx*, widget_id widget, utf8_string key, char* buf, uint64_t buf_size, uint64_t* size_needed_out);
error get_property_real(struct ctx*, widget_id widget, utf8_string key, float* value_out);
error get_property_int(struct ctx* widget_id widget, utf8_string key, uint32_t* value_out);


//----- Input

// Note: need to flesh this out.  Need to pass input from the application into the library,
// and return any requested actions.  Need to support arbitrary input devices.
error input(struct ctx*);


//----- Update

// The UI doesn't have its own main loop, so the application must inform it of 
// the passage of time.  This is used to update animations and so on.
error update(float dt);


//----- Draw

// This needs to output either a sequence of drawing commands as per nuklear, or something more abstract
// to allow for greater customisation of what is displayed by the application
error draw(struct ctx*);
