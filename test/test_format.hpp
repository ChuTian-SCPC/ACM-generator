#include "test.hpp"
using namespace generator::all;

TEST_CASE("numeric format", "[format]") {
    init_gen();
    // format int range
    auto r1 = __format_to_int_range<int>("[10, 20]");
    CHECK((r1.first == 10 && r1.second == 20));
    auto r2 = __format_to_int_range<int>("[1, 1e9]");
    CHECK((r2.first == 1 && r2.second == 1000000000));
    auto r3 = __format_to_int_range<int>("[1, 3.3)");
    CHECK((r3.first == 1 && r3.second == 2));
}