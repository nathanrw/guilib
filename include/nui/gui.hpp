#ifndef NUI_GUI_HPP
#define NUI_GUI_HPP

#include <nui/types.h>
#include <nui/status.hpp>
#include <nui/utf8.hpp>
#include <string>

namespace nui {
    class gui;
    class ast;
}

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

    status get_widget_properties(nui_widget_id widget, nui::widget_properties*& out);


private:
};

#endif
