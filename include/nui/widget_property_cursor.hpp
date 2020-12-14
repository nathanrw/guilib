#ifndef NUI_WIDGET_PROPERTY_CURSOR_H
#define NUI_WIDGET_PROPERTY_CURSOR_H

#include <nui/gui.hpp>
#include <nui/property_cursor.hpp>
#include <memory>

namespace nui {
    class widget_property_cursor;
}

/// Property cursor implementation wrapping up a widget's properties.
///
class nui::widget_property_cursor : public nui::property_cursor {
public:

    // Ctors / dtor

    widget_property_cursor(std::shared_ptr<widget> widget);
    widget_property_cursor(const widget_property_cursor& that) = delete;
    widget_property_cursor& operator=(const widget_property_cursor& that) = delete;
    virtual ~widget_property_cursor() override;

    // Overrides from property_cursor

    virtual const utf8_string& name() const override;
    virtual nui_int get_as_int() const override;
    virtual nui_real get_as_real() const override;
    virtual const utf8_string& get_as_string() const override;
    virtual bool get_as_bool() const override;
    virtual status get_as_data(nui_byte* buffer,
        size_t buffer_size,
        size_t& size_needed) override;
    virtual size_t number_of_children() const override;
    virtual status select_next_sibling() override;
    virtual status select_parent() override;
    virtual status select_child(nui_int index) override;
    virtual status select_named_child(const utf8_string& name) override;
    virtual status set_as_int(nui_int value) override;
    virtual status set_as_real(nui_real value) override;
    virtual status set_as_string(const utf8_string& value) override;
    virtual status set_as_bool(bool value) override;
    virtual status set_as_data(const nui_byte* buffer, size_t buffer_size) override;
    virtual status insert_before_selected(const utf8_string& name) override;
    virtual status insert_after_selected(const utf8_string& name) override;
    virtual status insert_into_selected(const utf8_string& name, size_t index) override;
    virtual status append_into_selected(const utf8_string& name) override;
    virtual status prepend_into_selected(const utf8_string& name) override;
    virtual status delete_selected() override;

private:
    // Widget is the widget whose properties the cursor is pointing at.   Property
    // stack is a stack of selected properties, with the selected property at the
    // end.
    struct selected_property {
        std::shared_ptr<property> m_property;
        size_t m_index;
    };

    selected_property* current_selection();
    const selected_property* current_selection() const;

    std::vector<std::shared_ptr<nui::property>>& current_selection_sibling_container() const;

    std::vector<std::shared_ptr<nui::property>>& current_selection_children_container() const;

    std::shared_ptr<widget> m_widget;
    std::vector<selected_property> m_property_stack;
};

#endif
