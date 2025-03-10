#ifndef _SGPCET_TEST_H_
#define _SGPCET_TEST_H_

#include "../generator.h"
#include "catch_amalgamated.hpp"

inline bool loop_check(std::function<bool()> func, int times) {
    for (int i = 0; i < times; i++) if (!func()) return false;
    return true;
}

inline int int_weight() { return generator::rand_numeric::rand_int(1, 100); }

#endif
