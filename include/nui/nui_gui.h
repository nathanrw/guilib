#ifndef NUI_GUI_H
#define NUI_GUI_H

#include "nui/nui_types.h"

NUI_API nui_bool nui_create_gui(
  struct nui_gui** out
);

NUI_API nui_bool nui_destroy_gui(
  struct nui_gui* gui
);

NUI_API enum nui_error_code nui_get_last_error_code(
  struct nui_gui* gui
);

NUI_API nui_utf8 nui_get_last_error_message(
  struct nui_gui* gui
);

NUI_API void nui_log_error(
  struct nui_gui* gui,
  enum nui_error_code code,
  nui_utf8 message
);

//----- Create and configure widgets

// When a widget is created, it is referencable by name or id, but is not visible - must
// set the 'visible' property to TRUE.

// Create widgets from text from a file
NUI_API nui_bool nui_create_widgets_from_file(
  struct nui_gui*,
  nui_widget_id parent,
  nui_utf8 filename
);

// Create widgets from text
NUI_API nui_bool nui_create_widgets_from_text(
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
NUI_API nui_bool nui_create_widget(
  struct nui_gui*,
  enum nui_widget_type type,
  nui_utf8 name,
  nui_widget_id parent,
  nui_widget_id* widget_id_out
);

// Get a list of child widgets.
NUI_API nui_bool nui_get_widget_children(
  struct nui_gui*,
  enum nui_widget_id,
  enum nui_widget_id* buf,
  nui_size buf_size,
  nui_size* size_needed_out
);

// Get a named widget.  The name can use 'dot' notation to navigate
// the hierarchy.
NUI_API nui_bool nui_get_widget(
  struct nui_gui*,
  nui_widget_id parent,
  nui_utf8 name,
  nui_widget_id* widget_id_out
);

// Get/set properties.
NUI_API nui_bool nui_get_widget_properties(
  struct nui_gui*,
  nui_widget_id widget,
  struct nui_data* out
);


//----- Input

// Note: need to flesh this out. Need to pass input from the application into
// the library, and return any requested actions. Need to support arbitrary
// input devices.
NUI_API bool nui_input(struct nui_gui*);


//----- Update

// The UI doesn't have its own main loop, so the application must inform it of
// the passage of time. This is used to update animations and so on.
NUI_API bool nui_update(struct nui_gui*, nui_real dt);


//----- Draw

// This needs to output either a sequence of drawing commands as per nuklear, or
// something more abstract to allow for greater customisation of what is
// displayed by the application
NUI_API bool nui_draw(struct nui_gui*);

#endif
