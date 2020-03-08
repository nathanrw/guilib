
#include "nui/nui_types.h"

#include "nui/impl/nui_gui.hpp"
#include "nui/impl/nui_status.hpp"

struct nui_gui {
    nui::gui m_gui;
    nui::status m_status;
};

bool nui_create_gui(struct nui_gui** out)
{
    assert(out);
    *out = new nui_gui;
}

bool nui_destroy_gui(struct nui_gui* gui)
{
    assert(gui);
    delete gui;
}

enum nui_error_code nui_get_last_error_code(struct nui_gui* gui)
{
    assert(gui);

    return gui->m_status.error_code();
}

nui_utf8 nui_get_last_error_message(struct nui_gui* gui)
{
    assert(gui);

    return gui->m_status.error_message().c_str();
}

void nui_log_error(
    struct nui_gui* gui,
    enum nui_error_code code,
    nui_utf8 message
)
{
    assert(gui);

    gui->m_status = nui::status(code, message);
}

bool nui_create_widgets_from_file(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 filename
)
{
    assert(gui);

    gui->m_status = m_gui.create_widgets_from_file(parent, filename);

    return !!gui->m_status;
}

bool nui_create_widgets_from_text(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 text
)
{
    assert(gui);

    gui->m_status = gui->m_gui.create_widgets_from_text(parent, text);

    return !!gui->m_status;
}

bool nui_create_widget(
    struct nui_gui* gui,
    enum nui_widget_type type,
    nui_utf8 name,
    nui_widget_id parent,
    nui_widget_id* widget_id_out
)
{
    assert(gui);
    assert(widget_id_out);

    gui->m_status =  gui->m_gui.create_widget(type, name, parent, *widget_id_out);

    return !!gui->m_status;
}

bool nui_get_widget_children(
    struct nui_gui* gui,
    enum nui_widget_id id,
    enum nui_widget_id* buf,
    nui_size buf_size,
    nui_size* size_needed_out
)
{
    assert(gui);
    assert(buf);
    assert(size_needed_out);

    std::vector<nui_widget_id> ids;
    gui->m_status = gui->m_gui.get_widget_children(id, ids);
    if (buf_size >= ids.size()) {
        memcpy_s(buf, buf_size, ids.data(), ids.size());
    } else {
        gui->m_status = nui::status(
            NUI_ERROR_BUFFER_TOO_SMALL,
            "Buffer too small."
        );
    }
    *size_needed_out = ids.size();

    return !!gui->m_status;
}

bool nui_get_widget(
    struct nui_gui* gui,
    nui_widget_id parent,
    nui_utf8 name,
    nui_widget_id* widget_id_out
)
{
    assert(gui);
    assert(widget_id_out);

    gui->m_status = gui->m_gui.get_widget(parent, name, *widget_id_out);

    return !!gui->m_status;
}

bool nui_get_widget_properties(
    struct nui_gui* gui,
    nui_widget_id widget,
    struct nui_data* out
)
{
    assert(gui);
    assert(out);

    nui::widget_properties* props = 0;
    gui->m_status = gui->get_widget_properties(widget, props);
    out->handle = props;
    out->vtable = nui::widget_properties::vtable();

    return !!gui->m_status;
}
