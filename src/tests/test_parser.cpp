#include <nui/ast.hpp>
#include <nui/parser.hpp>
#include <iostream>

#include <catch2/catch.hpp>

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

TEST_CASE("test_parser") {
    nui::parser parser;
    nui::ast ast;
    nui::status ok = parser.parse(main_menu, ast);
    CHECK(ok);
    if (!ok) {
        std::cout << ok.error_message() << std::endl;
    }
    nui::ast expected = nui::ast::make_root({
        nui::ast::make_widget(
            "window",
            "main_menu",
            {
                nui::ast::make_property("width", "400"),
                nui::ast::make_property("height", "300"),
                nui::ast::make_property("show_frame", "false"),
                nui::ast::make_widget(
                    "button",
                    "new_game",
                    {
                        nui::ast::make_property("text", "New Game"),
                        nui::ast::make_property("action", "NEW_GAME")
                    }
                ),
                nui::ast::make_widget(
                    "button",
                    "quit",
                    {
                        nui::ast::make_property("text", "Quit"),
                        nui::ast::make_property("action", "QUIT")
                    }
                )
            }
        )
    });
    CHECK(ast == expected);
}
