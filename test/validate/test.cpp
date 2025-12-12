#include "../../generator.h"
#include "catch_amalgamated.hpp"
using namespace generator::all;

TEST_CASE("validate for linux", "[validate][linux]") {
    init_gen();
    make_inputs(1, 3, [&]() {
        println(rand_int(1, 10), rand_int(1, 10), 2, "abc");
    });
    make_inputs(4, 5, [&]() {
        println(rand_even(1, 1000000), rand_even(1, 1000000));
    });
    Path folder = __validate_folder("testcases");
    __create_directories(folder);
#ifdef ON_WINDOWS
    CommandPath val(__path_join(__current_path(), "val.exe"));
#else
    CommandPath val(__path_join(__current_path(), "val"));
#endif
    for (int i = 1; i <= 5; i++) {
        Path log = __path_join(folder, __end_with(i, _VAL));
        log.__ensure_file_exist();
        std::cout << log.cname() << std::endl;
        Path input = __input_file_path(__path_join(__current_path(), "testcases"), i);
        ReturnState state = val.__run_program(input, _default_path, log, time_limit_inf, _VALIDATOR);
        if (i <= 3) CHECK(!__is_success(state.exit_code));
        else CHECK(__is_success(state.exit_code));
    }

}