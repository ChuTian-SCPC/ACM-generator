#ifndef _SGPCET_CHECKER_H_
#define _SGPCET_CHECKER_H_

#ifndef _SGPCET_PROGRAM_H_
#include "program.h"
#endif // !_SGPCET_PROGRAM_H_
#ifndef _SGPCET_IO_INIT_H_
#include "io_init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {
        class _Checker {
        protected:
            _Program* _checker;
            Path _input;
            Path _std_output;
            Path _ans_output;
            Path _testlib_output;
            _enum::_JudgeState _result;
            int _time_limit;
        public:
            _Checker() : _checker(nullptr), _result(_enum::_JudgeState::_UNKNOWN), _time_limit(_setting::time_limit_inf) {}

            template<typename T>
            _Checker(T&& checker) : _checker(nullptr), _result(_enum::_JudgeState::_UNKNOWN), _time_limit(_setting::time_limit_inf) {
                __set_checker(std::forward<T>(checker));
            }

            ~_Checker() {
                if (_checker != nullptr) delete _checker;
            }

            _GET_VALUE(_Program*, checker);
            _SET_GET_VALUE(Path, input);
            _SET_GET_VALUE(Path, std_output);
            _SET_GET_VALUE(Path, ans_output);
            _SET_GET_VALUE(Path, testlib_output);
            _SET_GET_VALUE(_enum::_JudgeState, result);
            _SET_GET_VALUE(int, time_limit);

            void __change_case(Path input, Path std_output, Path ans_output, Path testlib_output) {
                _input = input;
                _std_output = std_output;
                _ans_output = ans_output;
                _testlib_output = testlib_output;
            }

            template<typename T>
            void __set_checker(T&& checker) {
                if (_checker != nullptr) delete _checker;
                _checker = __checker_program(std::forward<T>(checker));
            }

            void __check_result() {
                _result = _enum::_JudgeState::_UNKNOWN;
                std::string testlib_output_content = "";
                _Program* p;
                if (_checker->enable_default_args()) 
                    p = _checker->__clone(tools::string_join(" ", _input, _ans_output, _std_output));
                else 
                    p = _checker->__clone();
                ReturnState res = p->__run_program(_setting::_default_path, _setting::_default_path, _testlib_output, _time_limit, _enum::_FuncProgramType::_CHECKER);
                if (__is_error(res.exit_code)) {
                    _result = _enum::_JudgeState::_ERROR;
                    return;
                }
                std::ifstream checker_stream(_testlib_output.path());
                std::string line;
                while(checker_stream >> line){
                    testlib_output_content += line;
                    testlib_output_content += " ";
                }
                checker_stream.close();
                if (testlib_output_content.substr(0, 2) == "ok") _result = _enum::_JudgeState::_AC;
                else _result = _enum::_JudgeState::_WA;
                delete p;
            }
        };
    } // namespace io
} // namespace generator

#endif // _SGPCET_CHECKER_H_
