#ifndef _SGPCET_GEN_STRATEGY_H_
#define _SGPCET_GEN_STRATEGY_H_

#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPECT_LOGGER_H_

namespace generator {
    namespace tools {
            class _Gen {
            public:
                _Gen(){}
                virtual void generate() {
                    _msg::__fail_msg(_msg::_defl, "unsupport generator.");
                };
            };

            template <typename T>
            class _BasicGen : public _Gen {
            protected:
                T& _context;
            public:
                _BasicGen(T& context) : _context(context) {}
            };

            class _GenSwitch {
            protected:
                _Gen* _generator;
            public:
                _GenSwitch() : _generator(nullptr) {}
                virtual ~_GenSwitch() { __delete_generator(); }

            protected:
                void __delete_generator() {
                    if (_generator) delete _generator;
                }
            };
    } // namespace tools
} // namespace generator

#endif // !_SGPCET_GEN_STRATEGY_H_