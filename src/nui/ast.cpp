#include <nui/ast.hpp>
#include <nui/assert.hpp>

nui::ast::ast()
    : m_node_type(root),
      m_name(""),
      m_type(""),
      m_value(""),
      m_children()
{
}

nui::ast::ast(const ast& that) = default;

nui::ast& nui::ast::operator=(const ast& that) = default;

nui::ast::~ast() = default;

nui::ast nui::ast::make_root(std::vector<ast> children)
{
    ast ret;
    ret.m_children = children;
    return ret;
}

nui::ast nui::ast::make_widget(
    const utf8_string& type,
    const utf8_string& name,
    std::vector<ast> children
)
{
    ast ret;
    ret.m_node_type = widget;
    ret.m_type = type;
    ret.m_name = name;
    ret.m_children = children;
    return ret;
}

nui::ast nui::ast::make_property(const utf8_string& name, const utf8_string& value)
{
    ast ret;
    ret.m_node_type = property;
    ret.m_name = name;
    ret.m_value = value;
    return ret;
}

nui::ast::type nui::ast::get_node_type() const
{
    return m_node_type;
}

const nui::utf8_string& nui::ast::get_name() const
{
    return m_name;
}

const nui::utf8_string& nui::ast::get_type() const
{
    return m_type;
}

const nui::utf8_string& nui::ast::get_value() const
{
    return m_value;
}

const std::vector<nui::ast>& nui::ast::get_children() const
{
    return m_children;
}

bool nui::ast::operator==(const nui::ast& that) const
{
    return 
        m_node_type == that.m_node_type &&
        m_name == that.m_name &&
        m_type == that.m_type &&
        m_value == that.m_value &&
        m_children == that.m_children;
}

static void print_node(std::ostream& os, const nui::ast& node, std::string indent)
{
    // todo: utf-8 handling.
    std::string node_type_str;
    switch (node.get_node_type()) {
    case nui::ast::root: node_type_str = "root"; break;
    case nui::ast::widget: node_type_str = "widget"; break;
    case nui::ast::property: node_type_str = "property"; break;
        default: NUI_ASSERT(false, "Unknown node type"); break;
    };
    std::string indent2 = indent + std::string("  ");
    std::string indent3 = indent2 + std::string("  ");
    os << indent << "{" << std::endl
        << indent2 << "node_type: " << node_type_str << std::endl
        << indent2 << "name: " << node.get_name() << std::endl
        << indent2 << "type: " << node.get_type() << std::endl
        << indent2 << "value: " << node.get_value() << std::endl
        << indent2 << "children: {" << std::endl;
    for (const nui::ast& child : node.get_children()) {
        print_node(os, child, indent3);
    }
    os << indent2 << "}" << std::endl
       << indent << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const nui::ast& node)
{
    print_node(os, node, "");
    return os;
}
