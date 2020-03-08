#include "nui/impl/nui_gui.hpp"

nui::gui::gui()
{
}

nui::gui::~gui()
{
}

nui::status nui::gui::create_widgets_from_file(nui_widget_id parent, const utf8_string& filename)
{
  // Convert the filename to a system mbcs so we can pass it to the ifstream
  // constructor.
  encodings::mbcs mbcs;
  std::string filename_mbcs;
  status ok = mbcs.from_utf8(filename, filename_mbcs);
  if (!ok) {
    return ok;
  }

  // Read the contents of the file as utf-8.  It is assumed that a valid gui file is
  // utf-8 encoded.
  utf8_string text;
  encodings::utf8 utf8;
  std::ifstream stream(filename_mbcs);
  text_reader reader;
  ok = text_reader.read(utf8, stream, text);
  if (!ok) {
    return ok;
  }

  return create_widgets_from_text(parent, text);
}

nui::status nui::gui::create_widgets_from_text(nui_widget_id parent, const utf8_string& text)
{
  // Parse an AST from the text.
  parser parser;
  ast ast;
  ok = parser.parse(text, ast);
  if (!ok) {
    return ok;
  }

  return create_widgets_from_ast(ast);
}

nui::status nui::gui::create_widgets_from_ast(nui_widget_id parent, const ast& ast)
{
  nui_status ok;
  nui_widget_id current = parent;
  switch (ast.node_type()) {

    // AST root
    case ast::root: {
      for (const ast& child : ast.children()) {
        ok = create_widgets_from_ast(current, child);
        if (!ok) {
          return ok;
        }
      }
      break;
    }

    // Widget node
    case ast::widget: {

      // Parse the widget type from the string in the AST node.
      nui_widget_type t;
      ok = parse_widget_type(ast.type());
      if (!ok) {
        return ok;
      }

      // Create a widget of the appropriate type.
      nui_widget_id id = 0;
      ok = create_widget(t, ast.name(), parent, id);
      if (!ok) {
        return ok;
      }

      // Create sub-widgets and set properties.
      for (const ast& child : ast.children()) {
        ok = create_widgets_from_ast(id, child);
        if (!ok) {
          return ok;
        }
      }
      break;
    }
    case ast::property: {
      // TODO: Set named property of `current`
      break;
    }
    default:
      assert(false); // unknown type
      break;
  }

  return ok;
}

nui::status nui::gui::create_widget(
  nui_widget_type type,
  const utf8_string& name,
  nui_widget_id parent,
  nui_widget_id& widget_id_out
)
{
  assert(false); // TODO
}

nui::status nui::gui::get_widget_children(nui_widget_id id, std::vector<nui_widget_id>& out)
{
  assert(false); // TODO
}

nui::status nui::gui::get_widget(const utf8_string& name, nui_widget_id& out)
{
  assert(false); // TODO
}

nui::status nui::gui::get_widget_properties(nui_widget_id widget, nui::widget_properties*& out)
{
  assert(false); // TODO
}
