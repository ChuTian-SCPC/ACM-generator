
#include "../generator.h"
#include "../catch_main.hpp"

using namespace generator::rand;

TEST_CASE("rand_odd", "[rand]") {
    int run_times = 100;
    for (int i = 1; i <= run_times; i++) {
      int x = rand_odd(3);
      REQUIRE((x >= 1 && x <= 3 && x%2));
      x = rand_odd(4);
      REQUIRE((x >= 1 && x <= 3 && x%2));
      x = rand_odd(0, 3);
      REQUIRE((x >= 1 && x <= 3 && x%2)); 
      x = rand_odd(0, 2);
      REQUIRE(x == 1);
      x = rand_odd(-3, 0);
      REQUIRE((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-4, 0);
      REQUIRE((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-2, 0);
      REQUIRE(x == -1);
    }
}



