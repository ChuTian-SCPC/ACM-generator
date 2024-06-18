#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "generator.h"

int main(int argc, char* argv[]) {

    generator::io::init_gen(argc, argv);

    int result = Catch::Session().run( argc, argv );

    return result;
}