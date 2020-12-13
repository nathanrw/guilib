#include <catch2/catch.hpp>
#include <nui/ast.hpp>

TEST_CASE("test_ast_make_root") {
    SECTION("empty") {
        nui::ast tree = nui::ast::make_root({});
        CHECK(tree.get_node_type() == nui::ast::root);
        CHECK(tree.get_children().size() == 0);
        CHECK(tree.get_type() == nui::utf8_string(""));
        CHECK(tree.get_value() == nui::utf8_string(""));
    }
    SECTION("with widgets") {
        nui::ast widget = nui::ast::make_widget("button", "jim", {});
        nui::ast tree = nui::ast::make_root({ widget });
        CHECK(tree.get_children().size() == 1);
    }
}

TEST_CASE("test_ast_make_widget") {
    REQUIRE(true);
    CHECK(true);
}

TEST_CASE("test_ast_make_property") {
    REQUIRE(true);
    CHECK(true);
}
