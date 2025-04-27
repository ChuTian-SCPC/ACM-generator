#pragma once
#include "test_basic.hpp"
#include "algorithm/geometry_checker.h"
using namespace generator::all;

TEST_CASE("geometry format","[rand_geometry][format]") {
    init_gen();
    auto p1 = __format_xy_range("[-100, 100]");
    CHECK((p1.first == "[-100, 100]" && p1.second == "[-100, 100]"));
    auto p2 = __format_xy_range("[-100, 100] [-200, 200]");
    CHECK((p2.first == "[-100, 100]" && p2.second == "[-200, 200]"));
    auto p3 = __format_xy_range("Y[-100, 100] x[-200, 200]");
    CHECK((p3.first == "[-200, 200]" && p3.second == "[-100, 100]"));
    auto p4 = __format_xy_range("[-100, 100] x[-200, 200]");
    CHECK((p4.first == "[-200, 200]" && p4.second == "[-100, 100]"));
    auto p5 = __format_xy_range("y[-100, 100] [-200, 200]");
    CHECK((p5.first == "[-200, 200]" && p5.second == "[-100, 100]"));
}

bool convex_hull_check() {
    int n = rand_int(1, 100000);
    int limit = n * rand_int(10, 100);
    ConvexHull<int> c(n); 
    c.set_xy_limit(-n, n);
    c.gen();
    return is_convex_hull(c);
}

TEST_CASE("rand convex hull", "[rand_geometry][ConvexHull]") {
    init_gen();
    bool f = loop_check([]() { return convex_hull_check();}, 10);
    CHECK(f);
}

bool simple_polygon_check() {
    int n = rand_int(1, 10000);
    int limit = n * rand_int(10, 100);
    SimplePolygon<int> p(n);
    p.set_xy_limit(-n, n);
    p.gen();
    return is_simple_polygon(p); 
}

TEST_CASE("rand simple polygon", "[rand_geometry][SimplePolygon]") {
    init_gen();
    bool f = loop_check([]() { return simple_polygon_check();}, 10);
    CHECK(f); 
}

class ConvexHullCheck : public ConvexHullGen<int> {
public:
    using Context = ConvexHullGen<int>::Context;
    ConvexHullCheck(Context& points) : ConvexHullGen<int>(points) {}
    std::vector<Point<int>> rand_no_origin_points(std::vector<int>& x_vec, std::vector<int>& y_vec) {
        return __rand_no_origin_points(x_vec, y_vec);
    }
};

TEST_CASE("convex hull rand no origin points", "[rand_geometry][ConvexHull]") {
    init_gen();
    ConvexHull<int> c;
    c.set_geometry_generator(new ConvexHullCheck(c));
    std::vector<int> x_vec = {0, 0, 0, 1, 2, 3};
    std::vector<int> y_vec = {0, 0, 0, 4, 5, 6};
    auto points = dynamic_cast<ConvexHullCheck*>(c.generator())->rand_no_origin_points(x_vec, y_vec);
    for (auto& p : points) CHECK(p != Point<int>(0, 0));
    x_vec = {0, 0, 0, -1, -2, -3};
    y_vec = {0, 0, 0, 4, 5, 6};
    points = dynamic_cast<ConvexHullCheck*>(c.generator())->rand_no_origin_points(x_vec, y_vec);
    for (auto& p : points) CHECK(p!= Point<int>(0, 0));
}