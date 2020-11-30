#ifndef NUI_GUI_HPP
#define NUI_GUI_HPP

#include <nui/types.h>
#include <nui/status.hpp>
#include <nui/utf8.hpp>
#include <string>

namespace nui {
    class ast;
    class gui;
    class property;
    class widget;
}


class nui::property {
private:
    utf8_string m_name;
    utf8_string m_value_string;
    nui_int m_value_int;
    nui_real m_value_real;
    std::vector<std::shared_ptr<property>> m_children;
};


class nui::widget {
public:
private:
    nui_widget_id m_widget_id;
    widget* m_parent;
    std::vector<std::shared_ptr<widget>> m_children;
    std::vector<std::shared_ptr<property>> m_properties;
};


class nui::gui {
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
    std::vector<std::shared_ptr<widget>> m_widgets;
};


#endif