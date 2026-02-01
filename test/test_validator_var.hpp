#include "test_basic.hpp"

using namespace validate::all;

TEST_CASE("auto name", "[validator][auto_name]") {
    auto s1 = _AutoName<int>().name();
    CHECK(s1 == "<unnamed>int_1");
    auto s2 = _AutoName<std::map<int, unsigned int>>().name();
    CHECK(s2 == "<unnamed>map<int,uint>_1");
    auto s3 = _AutoName<std::set<int>>().name();
    CHECK(s3 == "<unnamed>set<int>_1");
    auto s4 = _AutoName<int>().name();
    CHECK(s4 == "<unnamed>int_2");
    auto s5 = _AutoName<std::tuple<int, char, bool, std::string, std::pair<std::vector<int>, int>>>().name();
    CHECK(s5 == "<unnamed>tuple<int,char,bool,string,pair<vector<int>,int>>_1");    
    auto s6 = _AutoName<float>().name();
    CHECK(s6 == "<unnamed>float_1");
    auto s7 = _AutoName<std::vector<double>>().name();
    CHECK(s7 == "<unnamed>vector<double>_1");
    auto s8 = _AutoName<double>().name();
    CHECK(s8 == "<unnamed>double_1");
    auto s9 = _AutoName<std::vector<long long>>().name();
    CHECK(s9 == "<unnamed>vector<longlong>_1");
    auto s10 = _AutoName<void>().name();
    CHECK(s10 == "<unnamed>var_1");
}