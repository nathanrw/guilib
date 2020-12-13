#include <nui/gui.hpp>
#include <nui/ast.hpp>
#include <nui/parser.hpp>
#include <nui/property_cursor.hpp>
#include <nui/widget_property_cursor.hpp>
#include <nui/assert.hpp>
#include <fstream>
#include <map>

// widget

nui::widget::widget(nui_widget_id id, widget* parent)
    : m_widget_id(id),
      m_parent(parent)
{
}

nui::widget::~widget() = default;

// gui

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

    // Property node
    case ast::property: {
      std::shared_ptr<property_cursor> props = get_widget_properties(current);
      status prop_status = props->select_named_child(tree.get_name());
      if (!prop_status) {
        // 1. Might want to be more tolerant?
        // 2. Bailing leaves gui in intermediate state.
        return prop_status;
      }
      prop_status = props->set_as_string(tree.get_value());
      if (!prop_status) {
        // See above
        return prop_status;
      }
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
  nui_widget_id parent_id,
  nui_widget_id& widget_id_out
)
{
    std::shared_ptr<widget> parent = get_widget(parent_id);
    if (!parent) {
        return status(NUI_ERROR_INVALID_ARGUMENT, "Parent widget does not exist");
    }
    widget_id_out = m_widgets.size() + 1;
    m_widgets.push_back(std::make_shared<widget>(widget_id_out, parent.get()));

    return status::OK;
}

nui::status nui::gui::get_widget_children(nui_widget_id id, std::vector<nui_widget_id>& out)
{
    std::shared_ptr<widget> w = get_widget(id);
    if (!w) {
        return status(NUI_ERROR_INVALID_ARGUMENT, "Widget does not exist");
    }
    for (std::shared_ptr<widget> c : w->m_children) {
        out.push_back(c->m_widget_id);
    }
    return status::OK;
}

nui::status nui::gui::get_widget(const utf8_string& name, nui_widget_id& out)
{
    for (std::shared_ptr<widget> w : m_widgets) {
        std::shared_ptr<property_cursor> props = get_widget_properties(w->m_widget_id);
        status s = props->select_named_child("full_name");
        if (!s) {
            return s;
        }
        if (props->get_as_string() == name) {
            out = w->m_widget_id;
            return status::OK;
        }
    }

    return status(NUI_ERROR_FAILED, "Named widget does not exist.");
}

std::shared_ptr<nui::widget> nui::gui::get_widget(nui_widget_id id) const
{
    if (id < 0 || id > m_widgets.size()) {
        return nullptr;
    }

    std::shared_ptr<widget> w = m_widgets[id];
    NUI_ASSERT(
        !w || w->m_widget_id == id, 
        "Association between widget position and id must be maintained."
    );

    return w;
}

nui::status nui::gui::parse_widget_type(const utf8_string& type_str, nui_widget_type& out) const
{
    static std::map<utf8_string, nui_widget_type> table;
    if (table.empty()) {
#define NUI_WIDGET_TYPE_BEGIN
#define NUI_WIDGET_TYPE_ENTRY(NAME, STRNAME) table[STRNAME] = NUI_WIDGET_TYPE_##NAME;
#define NUI_WIDGET_TYPE_END
#include <nui/widget_type.h>
    }
    auto it = table.find(type_str);
    if (it != end(table)) {
        out = it->second;
        return status::OK;
    }
    return status(NUI_ERROR_FAILED, "Failed to parse widget type");
}

std::shared_ptr<nui::property_cursor> nui::gui::get_widget_properties(nui_widget_id id)
{
    std::shared_ptr<widget> w = get_widget(id);
    if (!w) {
        return nullptr;
    }
    return std::make_shared<nui::widget_property_cursor>(w);
}
