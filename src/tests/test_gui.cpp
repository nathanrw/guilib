#include <catch2/catch.hpp>
#include <nui/gui.hpp>
#include <nui/property_cursor.hpp>

#include <iostream>
#include <vector>

const nui::utf8_string main_menu =
"// comment 1\n"
"window main_menu { // comment 2 \n"
"  width: 400\n"
"  height: 300\n"
"  show_frame: false\n"
"  button new_game {\n"
"    text: \"New Game\"\n"
"    action: 'NEW_GAME'\n"
"  }\n"
"  button quit {\n"
"    text: 'Quit'\n"
"    action: QUIT\n"
"  }\n"
"}";

static void print_widgets(nui::gui& gui, nui_widget_id id)
{
    std::vector<nui_widget_id> ids;
    nui::status status = gui.get_widget_children(id, ids);
    REQUIRE(status);
    for (nui_widget_id id : ids) {
        std::cout << "widget id " << id << std::endl;
        std::shared_ptr<nui::property_cursor> props = gui.get_widget_properties(id);
        size_t num_props = props->number_of_children();
        for (size_t i = 0; i < num_props; ++i) {
            status = props->select_child(i);
            REQUIRE(status);
            std::cout << props->name() << " " << props->get_as_string() << std::endl;
            status = props->select_parent();
            REQUIRE(status);
        }
        std::cout << std::endl;
        print_widgets(gui, id);
    }
}

TEST_CASE("test_gui") {
    nui::gui gui;
    nui::status status = gui.create_widgets_from_text(0, main_menu);
    REQUIRE(status);
    print_widgets(gui, 0);
}
