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
                _msg::OutStream geometry_log(false);
                _msg::_defl.swap(geometry_log);                        
                __init();
                __generate_geometry();
                _msg::_defl.swap(geometry_log);
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
                    _msg::__fail_msg(_msg::_defl, tools::string_format( "range [%s, %s] for x-coordinate is invalid.",
                        std::to_string(x_left_limit).c_str(), std::to_string(x_right_limit).c_str()));
                if (y_left_limit > y_right_limit)
                    _msg::__fail_msg(_msg::_defl, tools::string_format( "range [%s, %s] for y-coordinate is invalid.",
                        std::to_string(y_left_limit).c_str(), std::to_string(y_right_limit).c_str()));
            }

            virtual void __self_init() {}

            virtual void __judge_self_limit() {}

            virtual void __generate_geometry() {
                _msg::__fail_msg(_msg::_defl, "unsupport geometry generator.");
            };
        };

        class _GeometryGenSwitch : public tools::_GenSwitch {
        public:
            void set_geometry_generator(tools::_Gen* gen) {
                __delete_generator();
                _generator = gen;
            }
        };
    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_GEOMETRY_STRATEGY_H_
