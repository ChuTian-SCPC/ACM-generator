#include "test_basic.hpp"

using namespace validate::all;

TEST_CASE("string to int", "[validator][string_convert][int]") {
    init_gen();
    _enum::StringConvertError err;
    int x = __string_convert<int>("123", err);
    CHECK(x == 123);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // max int
    x = __string_convert<int>("2147483647", err);
    CHECK(x == 2147483647);
    CHECK(err == _enum::StringConvertError::SUCCESS);
    
    // min int
    x = __string_convert<int>("-2147483648", err);
    CHECK(x == -2147483648);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // max int + 1
    x = __string_convert<int>("2147483648", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // min int - 1
    x = __string_convert<int>("-2147483649", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // len out of range
    x = __string_convert<int>("12345678901234567890", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // same len with max int
    x = __string_convert<int>("1234567890", err);
    CHECK(x == 1234567890);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // leading zero
    x = __string_convert<int>("0123", err);
    CHECK(x == 123);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // leading zero with negative number
    x = __string_convert<int>("-0123", err);
    CHECK(x == -123);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // leading zero len large than len(max_int)
    x = __string_convert<int>("000001234567890", err);
    CHECK(x == 1234567890);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // zero
    x = __string_convert<int>("0", err);
    CHECK(x == 0);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // negative zero
    x = __string_convert<int>("-0", err);
    CHECK(x == 0);
    CHECK(err == _enum::StringConvertError::NEGATIVE_ZERO);

    // invalid charset
    x = __string_convert<int>("47abc", err);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    // long long max
    long long y = __string_convert<long long>("9223372036854775807", err);
    CHECK(y == 9223372036854775807);
    CHECK(err == _enum::StringConvertError::SUCCESS);
    
    // long long min
    y = __string_convert<long long>("-9223372036854775808", err);
    CHECK(y == -9223372036854775808);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // long long max + 1
    y = __string_convert<long long>("9223372036854775808", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // long long min - 1
    y = __string_convert<long long>("-9223372036854775809", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // unsigned int max
    unsigned int z = __string_convert<unsigned int>("4294967295", err);
    CHECK(z == 4294967295);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // unsigned int max + 1
    z = __string_convert<unsigned int>("4294967296", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // unsigned int has negative number
    z = __string_convert<unsigned int>("-1", err);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);
    
}