#pragma once

#include "../generator.h"
#include "catch_amalgamated.hpp"
using namespace generator::all;

inline bool loop_check(std::function<bool()> func, int times) {
    for (int i = 0; i < times; i++) if (!func()) return false;
    return true;
}

inline int int_weight() { return rand_int(1, 100); }

