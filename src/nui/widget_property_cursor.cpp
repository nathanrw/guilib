#include <nui/widget_property_cursor.hpp>
#include <nui/assert.hpp>
#include <nui/value_parser.hpp>

nui::widget_property_cursor::widget_property_cursor(std::shared_ptr<widget> widget)
    : m_widget(widget)
{
    NUI_PRE(widget, "Widget must not be null");
}

nui::widget_property_cursor::~widget_property_cursor() = default;

const nui::utf8_string& nui::widget_property_cursor::name() const
{
    const selected_property* prop = current_selection();
    if (!prop) {
        static utf8_string empty;
        return empty;
    }
    return prop->m_property->m_name;
}

nui_int nui::widget_property_cursor::get_as_int() const
{
    const selected_property* prop = current_selection();
    if (!prop) return 0;
    return prop->m_property->m_value_int;
}

nui_real nui::widget_property_cursor::get_as_real() const
{
    const selected_property* prop = current_selection();
    if (!prop) return 0;
    return prop->m_property->m_value_real;
}

const nui::utf8_string& nui::widget_property_cursor::get_as_string() const
{
    const selected_property* prop = current_selection();
    static utf8_string empty;
    if (!prop) return empty;
    return prop->m_property->m_value_string;
}

bool nui::widget_property_cursor::get_as_bool() const
{
    return !!get_as_int();
}

nui::status nui::widget_property_cursor::get_as_data(
    nui_byte* buffer,
    size_t buffer_size,
    size_t& size_needed)
{
    return status(NUI_ERROR_FAILED, "Property not convertable to buffer");
}

size_t nui::widget_property_cursor::number_of_children() const
{
    return current_selection_children_container().size();
}

nui::status nui::widget_property_cursor::select_next_sibling()
{
    if (m_property_stack.size() == 0) {
        return status(NUI_ERROR_FAILED, "No sibling to select");
    }

    selected_property* prop = current_selection();
    std::vector<std::shared_ptr<property>>& container = current_selection_sibling_container();

    if (prop->m_index >= container.size()) {
        return status(NUI_ERROR_FAILED, "No further siblings to select");
    }
    prop->m_index++;
    prop->m_property = container[prop->m_index];

    return status::OK;
}

nui::status nui::widget_property_cursor::select_parent()
{
    if (m_property_stack.size() == 0) {
        return status(NUI_ERROR_FAILED, "No parent to select");
    }
    m_property_stack.resize(m_property_stack.size() - 1);
    return status::OK;
}

nui::status nui::widget_property_cursor::select_child(nui_int index)
{
    std::vector<std::shared_ptr<property>>& container = current_selection_children_container();
    if (index < 0 || index >= (nui_int)container.size()) {
        return status(NUI_ERROR_FAILED, "Index out of bounds");
    }
    m_property_stack.push_back({ container[index], (size_t)index });
    return status::OK;
}

nui::status nui::widget_property_cursor::select_named_child(const utf8_string& name)
{
    std::vector<std::shared_ptr<property>>& container = current_selection_children_container();
    for (nui_int index = 0; index < (nui_int)container.size(); ++index) {
        if (container[index]->m_name == name) {
            m_property_stack.push_back({ container[index], (size_t)index });
            return status::OK;
        }
    }
    return status(NUI_ERROR_FAILED, "No such named property");
}

nui::status nui::widget_property_cursor::set_as_int(nui_int value)
{
    selected_property* prop = current_selection();
    if (prop) {
        prop->m_property->m_value_int = value;
        prop->m_property->m_value_real = nui_real(value);
        prop->m_property->m_value_string = value_parser::to_string(value);
        return status::OK;
    }
    return status(NUI_ERROR_FAILED, "No selection");
}

nui::status nui::widget_property_cursor::set_as_real(nui_real value)
{
    selected_property* prop = current_selection();
    if (prop) {
        prop->m_property->m_value_int = nui_int(value);
        prop->m_property->m_value_real = value;
        prop->m_property->m_value_string = value_parser::to_string(value);
        return status::OK;
    }
    return status(NUI_ERROR_FAILED, "No selection");
}

nui::status nui::widget_property_cursor::set_as_string(const utf8_string& value)
{
    nui_real real_value;
    status is_numeric = value_parser::parse_real(value, real_value);
    if (is_numeric) {
        return set_as_real(real_value);
    }

    selected_property* prop = current_selection();
    if (prop) {
        int bool_value = value_parser::eval_bool(value) ? 1 : 0;
        prop->m_property->m_value_int = bool_value;
        prop->m_property->m_value_real = bool_value;
        prop->m_property->m_value_string = value;
        return status::OK;
    }
    return status(NUI_ERROR_FAILED, "No selection");
}

nui::status nui::widget_property_cursor::set_as_bool(bool value)
{
    return set_as_int(value ? 1 : 0);
}

nui::status nui::widget_property_cursor::set_as_data(const nui_byte* buffer, size_t buffer_size)
{
    return status(NUI_ERROR_FAILED, "Not representable as buffer");
}

std::vector<std::shared_ptr<nui::property>>& nui::widget_property_cursor::current_selection_sibling_container() const
{
    static std::vector<std::shared_ptr<nui::property>> empty;
    if (m_property_stack.size() == 0) {
        return empty;
    }
    if (m_property_stack.size() == 1) {
        return m_widget->m_properties;
    }
    else {
        return m_property_stack[m_property_stack.size() - 2].m_property->m_children;
    }
}

 std::vector<std::shared_ptr<nui::property>>& nui::widget_property_cursor::current_selection_children_container() const
 {
     if (m_property_stack.size() == 0) {
         return m_widget->m_properties;
     } else {
         return m_property_stack.back().m_property->m_children;
     }
 }

nui::widget_property_cursor::selected_property* nui::widget_property_cursor::current_selection()
{
    if (m_property_stack.size() == 0) {
        return nullptr;
    }
    return &m_property_stack.back();
}

const nui::widget_property_cursor::selected_property* nui::widget_property_cursor::current_selection() const
{
    return const_cast<widget_property_cursor*>(this)->current_selection();
}
