#include <nui/gui.hpp>
#include <nui/ast.hpp>
#include <nui/parser.hpp>
#include <nui/assert.hpp>
#include <fstream>

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
  ok = reader.read(utf8, stream, text);
  if (!ok) {
    return ok;
  }

  return create_widgets_from_text(parent, text);
}

nui::status nui::gui::create_widgets_from_text(nui_widget_id parent, const utf8_string& text)
{
  // Parse an AST from the text.
  parser parser_;
  ast ast;
  status ok = parser_.parse(text, ast);
  if (!ok) {
    return ok;
  }

  return create_widgets_from_ast(parent, ast);
}

nui::status nui::gui::create_widgets_from_ast(nui_widget_id parent, const ast& tree)
{
  status ok;
  nui_widget_id current = parent;
  switch (tree.get_node_type()) {

    // AST root
    case ast::root: {
      for (const ast& child : tree.get_children()) {
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
      ok = parse_widget_type(tree.get_type(), t);
      if (!ok) {
        return ok;
      }

      // Create a widget of the appropriate type.
      nui_widget_id id = 0;
      ok = create_widget(t, tree.get_name(), parent, id);
      if (!ok) {
        return ok;
      }

      // Create sub-widgets and set properties.
      for (const ast& child : tree.get_children()) {
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
      NUI_ASSERT(false, "Unknown node type.");
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
  return status(NUI_ERROR_UNKNOWN, "Not implemented");
}

nui::status nui::gui::get_widget_children(nui_widget_id id, std::vector<nui_widget_id>& out)
{
  return status(NUI_ERROR_UNKNOWN, "Not implemented");
}

nui::status nui::gui::get_widget(const utf8_string& name, nui_widget_id& out)
{
  return status(NUI_ERROR_UNKNOWN, "Not implemented");
}

nui::status nui::gui::parse_widget_type(const utf8_string& type_str, nui_widget_type& out) const
{
  return status(NUI_ERROR_UNKNOWN, "Failed to parse widget type");
}