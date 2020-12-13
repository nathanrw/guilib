#include <catch2/catch.hpp>
#include <nui/utf8.hpp>

#include <vector>

const nui::utf8_string main_menu =
"window main_menu {\n"
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

TEST_CASE("test_gui") {
    REQUIRE(true);
    CHECK(true);
}
