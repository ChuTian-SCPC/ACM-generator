#ifndef _SGPCET_VAL_INIT_H_
#define _SGPCET_VAL_INIT_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_

namespace generator {
    namespace validate {
        class _ValidateAutoReadEof {
        private:
            static bool _registered;

        public:
            static void _auto_read_eof() {
                if (testlibFinalizeGuard.readEofCount != 0) return;
                inf.readEof();
            }

            _ValidateAutoReadEof() {
                if (_registered) return;
                _registered = true;
                atexit(_auto_read_eof);
            }

            _ValidateAutoReadEof(const _ValidateAutoReadEof&) = delete;
            _ValidateAutoReadEof& operator=(const _ValidateAutoReadEof&) = delete;
        };
        bool _ValidateAutoReadEof::_registered = false;

        class _AutoName {
        private:
            static int _var_count;
        public:
            _AutoName() {
                _var_count++;
            }

            std::string name() {
                return tools::string_format("var_%d", _var_count);
            }
        };
        int _AutoName::_var_count = 0;
    }
};

#endif // !_SGPCET_VAL_INIT_H_
