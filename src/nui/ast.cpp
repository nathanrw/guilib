#include <nui/ast.hpp>

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