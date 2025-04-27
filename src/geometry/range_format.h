#ifndef _SGPCET_RANGE_FORMAT_H_
#define _SGPCET_RANGE_FORMAT_H_

#ifndef _SGPCET_GEOMETRY_BASIC_H_
#include "geometry_basic.h"
#endif //!_SGPCET_GEOMETRY_BASIC_H_
#ifndef _SGPCET_NUMBER_FORMAT_H_
#include "rand/number_format.h"
#endif //!_SGPCET_NUMBER_FORMAT_H_

namespace generator {
    namespace rand_geometry {
        struct _RangeString {
            std::string range;
            std::string type;
            _RangeString(std::string range = "", std::string type = "") : range(range), type(type) {}

            bool any_type() { return type == ""; }
            bool empty_range() { return range == ""; }
        };

        _RangeString __find_range(std::string s, size_t& pos_start, std::vector<std::string>& types) {
            size_t open = s.find_first_of("[(", pos_start);
            size_t close = s.find_first_of(")]", pos_start);
            if (open == std::string::npos && close == std::string::npos)
                return _RangeString();
            std::string range = s.substr(open, close - open + 1);
            if (open == std::string::npos || close == std::string::npos)
                _msg::__fail_msg(_msg::_defl, "%s is not a vaild range.", range.c_str());
            std::string mark = s.substr(pos_start, open - pos_start);
            pos_start = close + 1;
            std::string range_type = "";
            for (auto& type : types) {
                if (mark.find_first_of(type) != std::string::npos) {
                    if (range_type != "" && range_type != type)
                        return _RangeString(range);
                    range_type = type;
                }
            }
            return _RangeString(range, range_type);
        }

        std::pair<std::string, std::string> __format_xy_range(std::string format) {          
            std::vector<std::string> types = { "xX", "yY" };
            size_t pos_start = 0;
            std::vector<_RangeString> ranges;
            while (pos_start < format.length()) {
                _RangeString range = __find_range(format, pos_start, types);
                if (range.empty_range()) break;
                ranges.push_back(range);
            }
            if (ranges.size() == 0) _msg::__fail_msg(_msg::_defl, "no range found.");
            if (ranges.size() > 2) _msg::__fail_msg(_msg::_defl, "too many ranges found.");
            if (ranges.size() == 1) {
                if (!ranges[0].any_type()) 
                    _msg::__fail_msg(_msg::_defl, tools::string_format("only found %c range.", ranges[0].type[0]));
                return std::make_pair(ranges[0].range, ranges[0].range);
            }
            if (!ranges[0].any_type() && ranges[0].type == ranges[1].type) 
                _msg::__fail_msg(_msg::_defl, tools::string_format("found %c range twice.", ranges[0].type[0]));
            auto result = std::make_pair(ranges[0].range, ranges[1].range);
            // a a
            // a x -> x a
            // a y
            // x a
            // x y
            // y a -> a y
            // y x -> x y
            if (ranges[0].type == types[1] || ranges[1].type == types[0]) std::swap(result.first, result.second);
            return result;
        }
    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_RANGE_FORMAT_H_
