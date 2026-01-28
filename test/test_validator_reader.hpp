#include "test_basic.hpp"

using namespace validate::all;

TEST_CASE("string to int", "[validator][string_to_int]") {
    init_gen();
    _enum::StringConvertError err;
    int x = __string_to_int<int>("123", err);
    CHECK(x == 123);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // max int
    x = __string_to_int<int>("2147483647", err);
    CHECK(x == 2147483647);
    CHECK(err == _enum::StringConvertError::SUCCESS);
    
    // min int
    x = __string_to_int<int>("-2147483648", err);
    CHECK(x == -2147483648);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // max int + 1
    x = __string_to_int<int>("2147483648", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // min int - 1
    x = __string_to_int<int>("-2147483649", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // len out of range
    x = __string_to_int<int>("12345678901234567890", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // same len with max int
    x = __string_to_int<int>("1234567890", err);
    CHECK(x == 1234567890);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // leading zero
    x = __string_to_int<int>("0123", err);
    CHECK(x == 123);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // leading zero with negative number
    x = __string_to_int<int>("-0123", err);
    CHECK(x == -123);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // leading zero len large than len(max_int)
    x = __string_to_int<int>("000001234567890", err);
    CHECK(x == 1234567890);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    // zero
    x = __string_to_int<int>("0", err);
    CHECK(x == 0);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // negative zero
    x = __string_to_int<int>("-0", err);
    CHECK(x == 0);
    CHECK(err == _enum::StringConvertError::NEGATIVE_ZERO);

    // invalid charset
    x = __string_to_int<int>("47abc", err);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    // long long max
    long long y = __string_to_int<long long>("9223372036854775807", err);
    CHECK(y == 9223372036854775807);
    CHECK(err == _enum::StringConvertError::SUCCESS);
    
    // long long min
    y = __string_to_int<long long>("-9223372036854775808", err);
    CHECK(y == -9223372036854775808);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // long long max + 1
    y = __string_to_int<long long>("9223372036854775808", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // long long min - 1
    y = __string_to_int<long long>("-9223372036854775809", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // unsigned int max
    unsigned int z = __string_to_int<unsigned int>("4294967295", err);
    CHECK(z == 4294967295);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    // unsigned int max + 1
    z = __string_to_int<unsigned int>("4294967296", err);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

    // unsigned int has negative number
    z = __string_to_int<unsigned int>("-1", err);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);
    
}

TEST_CASE("string to double", "[validator][string_to_double]") {
    init_gen();
    using Catch::Matchers::WithinAbs;
    _enum::StringConvertError err;
    bool is_scientific = false;
    int point_digits_count = 0;
    double x = __string_to_double<double>("123.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(123.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("123.456e-2", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(123.456e-2, 1e-6));
    CHECK(is_scientific == true);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("123.456e2", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(12345.6, 0.01));
    CHECK(is_scientific == true);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("-123.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(-123.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("123.45.6e", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("-123.456-", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("123.456e2e3", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("123.e", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("123.", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("123e", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("e", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>(".", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::INVALID_FORMAT);

    x = __string_to_double<double>("0", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(0.0, 1e-6));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 0);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("-0", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(0.0, 1e-6));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 0);
    CHECK(err == _enum::StringConvertError::NEGATIVE_ZERO);

    x = __string_to_double<double>("-0123.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(-123.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    x = __string_to_double<double>("0123.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(123.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    x = __string_to_double<double>("0.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(0.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::SUCCESS);

    x = __string_to_double<double>("00.456", err, is_scientific, point_digits_count);
    CHECK_THAT(x, WithinAbs(0.456, 0.0001));
    CHECK(is_scientific == false);
    CHECK(point_digits_count == 3);
    CHECK(err == _enum::StringConvertError::LEADING_ZERO);

    x = __string_to_double<double>("1e9999", err, is_scientific, point_digits_count);
    CHECK(err == _enum::StringConvertError::OUT_OF_RANGE);

}