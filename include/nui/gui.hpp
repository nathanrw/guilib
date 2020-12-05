#ifndef NUI_GUI_HPP
#define NUI_GUI_HPP

#include <nui/types.h>
#include <nui/status.hpp>
#include <nui/utf8.hpp>
#include <string>
#include <memory>

namespace nui {
    class ast;
    class gui;
    class property;
    class property_cursor;
    class widget_property_cursor;
    class widget;
}


class nui::property {
public:
private:
    friend class nui::widget_property_cursor;
    utf8_string m_name;
    utf8_string m_value_string;
    nui_int m_value_int;
    nui_real m_value_real;
    std::vector<std::shared_ptr<property>> m_children;
};


class nui::widget {
public:
    widget(nui_widget_id id, widget* parent);
    widget(const widget& that) = delete;
    widget& operator=(const widget& that) = delete;
    ~widget();
private:
    friend class nui::gui;
    friend class nui::widget_property_cursor;
    nui_widget_id m_widget_id;
    widget* m_parent;
    std::vector<std::shared_ptr<widget>> m_children;
    std::vector<std::shared_ptr<property>> m_properties;
};


class NUI_API nui::gui {
public:


    gui();
    gui(const gui& that) = delete;
    gui& operator=(const gui& that) = delete;
    virtual ~gui();


    status create_widgets_from_file(nui_widget_id parent, const utf8_string& filename);

    status create_widgets_from_text(nui_widget_id parent, const utf8_string& text);

    status create_widgets_from_ast(nui_widget_id parent, const ast& ast);

    status create_widget(
        nui_widget_type type,
        const utf8_string& name,
        nui_widget_id parent,
        nui_widget_id& widget_id_out
    );

    status get_widget_children(nui_widget_id id, std::vector<nui_widget_id>& out);

    status get_widget(const utf8_string& name, nui_widget_id& out);

    std::shared_ptr<property_cursor> get_widget_properties(nui_widget_id widget);


private:

    std::shared_ptr<widget> get_widget(nui_widget_id id) const;

	status parse_widget_type(const utf8_string& type_string, nui_widget_type& t) const;

    std::vector<std::shared_ptr<widget>> m_widgets;
};


#endif
