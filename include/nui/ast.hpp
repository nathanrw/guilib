#ifndef NUI_AST_HPP
#define NUI_AST_HPP

#include <nui/types.h>
#include <nui/utf8.hpp>
#include <vector>

namespace nui {
    class ast;
}


/// An AST node for a synax like so:
///
///    file := [entry]...
///    entry := <prop>|<widget>
///    prop := <name>: <value>
///    widget := <type> [name] {
///      [entry]...
///    }
///    name := [a-zA-Z0-9_]+
///    value := *
///
/// This is either a root, a widget or a property.
///
/// All properties can be accessed regardless of the node type, but properties
/// which are not meaningful for a given node type will be 'empty' or otherwise
/// initialised to some sensible default value.
class nui::ast {
public:


    /// AST node type
    enum type { root, widget, property };


    /// A default constructed ast will be a root node.
    ast();

    // Copy.
    ast(const ast& that);

    // Assign.
    ast& operator=(const ast& that);

    // Destructor.
    ~ast();


    /// Factory function to make a root node.
    static ast make_root(std::vector<ast> children);

    /// Factory function to make a widget node.
    static ast make_widget(
        const utf8_string& type,
        const utf8_string& name,
        std::vector<ast> children
    );

    /// Factry function to make a property node.
    static ast make_property(const utf8_string& name, const utf8_string& value);


    /// Get the node type.
    type get_node_type() const;

    /// Get the node name. This is meaningful for widgets and properties.
    const utf8_string& get_name() const;

    // Get the widget type. This is meaningful for widgets.
    const utf8_string& get_type() const;

    /// Get the node value.  This is meaningful for properties.
    const utf8_string& get_value() const;

    /// Get the children of this node. This is meaningful for the root and for
    /// widgets.
    const std::vector<ast>& get_children() const;


    /// Compare for equality
    bool operator==(const ast& that) const;


private:
    type m_node_type;
    utf8_string m_name;
    utf8_string m_type;
    utf8_string m_value;
    std::vector<ast> m_children;
};


/// Stream operator for debugging.
std::ostream& operator<<(std::ostream& os, const nui::ast& node);


#endif
