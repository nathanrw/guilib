#include <catch2/catch.hpp>
#include <nui/value_parser.hpp>
#include <nui/status.hpp>

TEST_CASE("test_parse_real") {
    SECTION("Garbage") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("wibble", out);
        CHECK(!ok);
        CHECK(out == 0);
    }
    SECTION("Garbage with number at end") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("wibble45.3", out);
        CHECK(!ok);
        CHECK(out == 0);
    }
    SECTION("Garbage with number at start") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("45wibble", out);
        CHECK(!ok);
        CHECK(out == 45);
    }
    SECTION("Empty string") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("", out);
        CHECK(!ok);
        CHECK(out == 0);
    }
    SECTION("Zero") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("0", out);
        CHECK(ok);
        CHECK(out == 0);
    }
    SECTION("Zero dot zero f") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("0.0f", out);
        CHECK(!ok);
        CHECK(out == 0);
    }
    SECTION("Twelve") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("12", out);
        CHECK(ok);
        CHECK(out == 12);
    }
    SECTION("Twelve point five") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("12.5", out);
        CHECK(ok);
        CHECK(out == 12.5);
    }
    SECTION("Twelve comma five") {
        nui_real out = 0;
        nui::status ok = nui::value_parser::parse_real("12,5", out);
        CHECK(!ok);
        CHECK(out == 12);
    }
}

TEST_CASE("test_eval_bool") {
    CHECK(!nui::value_parser::eval_bool(""));
    CHECK(!nui::value_parser::eval_bool("false"));
    CHECK(nui::value_parser::eval_bool("true"));
    CHECK(nui::value_parser::eval_bool("the world is flat"));
    CHECK(nui::value_parser::eval_bool("False")); // case sensitive.
    CHECK(nui::value_parser::eval_bool("0")); // Not parsing numbers.
}

TEST_CASE("test_int_to_string") {
    CHECK(nui::value_parser::to_string((nui_int)1023) == nui::utf8_string("1023"));
    CHECK(nui::value_parser::to_string((nui_int)0) == nui::utf8_string("0"));
    CHECK(nui::value_parser::to_string((nui_int)-12) == nui::utf8_string("-12"));
}

TEST_CASE("test_real_to_string") {
    // Need to do something about float format as evidenced here. Locale too....
    CHECK(nui::value_parser::to_string((nui_real)1023) == nui::utf8_string("1023.000000"));
    CHECK(nui::value_parser::to_string((nui_real)0) == nui::utf8_string("0.000000"));
    CHECK(nui::value_parser::to_string((nui_real)-12) == nui::utf8_string("-12.000000"));
    CHECK(nui::value_parser::to_string((nui_real)-12.5) == nui::utf8_string("-12.500000"));
}
