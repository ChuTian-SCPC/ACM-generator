#ifndef _SGPCET_GEOMETRY_STRATEGY_H_
#define _SGPCET_GEOMETRY_STRATEGY_H_

#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_
#ifndef _SGPCET_GEN_STRATEGY_H_
#include "basic/gen_strategy.h"
#endif // !_SGPCET_GEN_STRATEGY_H_

namespace generator {
    namespace rand_geometry {
        template<template<typename> class GeoType, typename ValueType>
        class BasicGeometryGen : public tools::_BasicGen<GeoType<ValueType>> {
        public:
            BasicGeometryGen(GeoType<ValueType>& context) : tools::_BasicGen<GeoType<ValueType>>(context) {}
            virtual void generate() override {
                _CONTEXT_GET(log_change);
                _msg::OutStream geo_log(false);
                if (log_change) _msg::_defl.swap(geo_log);                        
                __init();
                __generate_geo();
                if (log_change) _msg::_defl.swap(geo_log);
            };
        protected:
            virtual void __init() {
                __self_init();
                __judge_limits();
            };

            void __judge_limits() {
                __judge_comman_limit();
                __judge_self_limit();
            } 

            virtual void __judge_comman_limit() {
                _CONTEXT_GET(x_left_limit);
                _CONTEXT_GET(x_right_limit);
                _CONTEXT_GET(y_left_limit);
                _CONTEXT_GET(y_right_limit);
                if (x_left_limit > x_right_limit)
                    _msg::__fail_msg(_msg::_defl, "x range is not vaild : x_left_limit > x_right_limit.");
                if (y_left_limit > y_right_limit)
                    _msg::__fail_msg(_msg::_defl, "y range is not vaild : y_left_limit > y_right_limit.");
            }

            virtual void __judge_self_limit() {}

            virtual void __generate_geo() {
                _msg::__fail_msg(_msg::_defl, "unsupport geometry generator.");
            };
        };
    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_GEOMETRY_STRATEGY_H_
