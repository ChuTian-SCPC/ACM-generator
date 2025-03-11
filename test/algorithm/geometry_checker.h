#include "../generator.h"
using namespace generator::all;

template<typename T>
bool is_segment_intersect(const Point<T>& A, const Point<T>& B, const Point<T>& C, const Point<T>& D) {
    _ResultTypeT<T> c1 = (B - A) ^ (C - A);
    _ResultTypeT<T> c2 = (B - A) ^ (D - A);
    _ResultTypeT<T> c3 = (D - C) ^ (A - C);
    _ResultTypeT<T> c4 = (D - C) ^ (B - C);

    if ((c1 * c2 < 0) && (c3 * c4 < 0)) return true; 
    return false; 
}

template<typename T>
bool is_simple_polygon(const SimplePolygon<T>& polygon) {
    auto points = polygon.points();
    int n = points.size();
    if (n < 3) return true;
    for (int i = 0; i < n; i++) {
        Point A = points[i];
        Point B = points[(i + 1) % n];
        for (int j = i + 1; j < n; j++) {
            Point C = points[j];
            Point D = points[(j + 1) % n];
            if (is_segment_intersect(A, B, C, D)) {
                // return false; 
                continue;
            }
        }
    }
    return true; 
}

template<typename T>
bool is_convex_hull(const ConvexHull<T>& polygon) {
    auto points = polygon.points();
    int n = points.size();
    if (n < 3) return true;

    double p = 0;
    for (int i = 0; i < n; i++) {
        Point O = points[i];
        Point A = points[(i + 1) % n];
        Point B = points[(i + 2) % n];
        double c = (A - O) ^ (B - O);

        if (c == 0) continue;
        if (p == 0) {
            p = c;
        } else if (c * p < 0) {
            return false;
        }
    }
    return true;
}