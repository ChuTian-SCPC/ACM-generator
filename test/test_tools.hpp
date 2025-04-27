#pragma once
#include "test_basic.hpp"
using namespace generator::all;

TEST_CASE("string format", "[tools][format]") {
    init_gen();
    std::string s = string_format("%d %c %s", 1, 'a', "abc");
    CHECK(s == "1 a abc");
}

TEST_CASE("string join", "[tools][format]") {
    init_gen();
    std::string s = string_join("/", 'a', "abc", 123);
    CHECK(s == "a/abc/123");
    std::string s2 = string_join("@", s, 1, "a");
    CHECK(s == "a/abc/123");
    CHECK(s2 == "a/abc/123@1@a");
}