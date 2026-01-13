#ifndef _SGPCET_TEST_VALIDATOR_H_
#define _SGPCET_TEST_VALIDATOR_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_

namespace generator {
    namespace io {
        class _TestValidator : public _Reporter {
        protected:
            using ExpectedResults = std::map<Path, _enum::ExpectedResult>;
            using TestResults = std::map<Path, ReturnState>;
            _Program* _validator;
            int _time_limit;
            ExpectedResults _testcases;
            TestResults _results;
        public:
            _TestValidator() : _validator(nullptr), _time_limit(_setting::time_limit_inf) {}

            template<typename T>
            _TestValidator(T&& validator) : _time_limit(_setting::time_limit_inf) {
                __set_validator(std::forward<T>(validator));
            }

            ~_TestValidator() {
                if (_validator) delete _validator;
            }

            _GET_VALUE(_Program*, validator);
            _SET_GET_VALUE(int, time_limit);
            _SET_GET_VALUE(ExpectedResults, testcases);
            _GET_VALUE(TestResults, results);

            template<typename T>
            void __set_validator(T&& validator) {
                if (_validator) delete _validator;
                _validator = __result_program(std::forward<T>(validator));
            }

            template<typename T>
            void __add_testcase(T&& path, _enum::ExpectedResult result = _enum::ExpectedResult::Any) {
                _testcases[Path(std::forward<T>(path))] = result;
            }

            bool __empty() {
                return _testcases.empty();
            }

            Path __log_name(Path path) {
                return __path_join(__test_validator_folder(), __end_with(path.__file_name(), _enum::_VAL));
            }

            void __run() {
                _validator->__check_program_valid();
                Path folder = __test_validator_folder();
                __create_directories(folder);
                int count = 1;
                for (auto& testcase : _testcases) {
                    Path input = testcase.first;
                    Path log = __log_name(input);
                    _msg::__flash_msg(_msg::_defl, "Test Validator : ", __ratio_msg(count, _testcases.size()));
                    _results[input] = _validator->__run_program(input, _setting::_default_path, log, _time_limit, _enum::_VALIDATOR);
                    count++;
                }
                _msg::__endl(_msg::_defl);
            }

            _msg::_ColorMsg __state_msg(ReturnState state) {
                if (!__is_success(state.exit_code)) return _fail_msg;
                if (__time_limit_exceed(state.time, _time_limit)) return _tle_msg;
                return _success_msg;
            }

            _msg::_ColorMsg __expected_msg(_enum::ExpectedResult result) {
                if (result == _enum::ExpectedResult::Any) return _msg::_ColorMsg("Any");
                if (result == _enum::ExpectedResult::Pass) return _success_msg;
                if (result == _enum::ExpectedResult::Fail) return _fail_msg;
                if (result == _enum::ExpectedResult::TLE) return _tle_msg;
                return _unknown_msg;
            }

            bool __is_pass(_enum::ExpectedResult expected, ReturnState state) {
                if (expected == _enum::ExpectedResult::Any) return true;
                if (expected == _enum::ExpectedResult::TLE) return __time_limit_exceed(state.time, _time_limit);
                if (expected == _enum::ExpectedResult::Pass) return __is_success(state.exit_code);
                if (expected == _enum::ExpectedResult::Fail) return !__is_success(state.exit_code);
                return false;
            }
            
            bool __is_all_pass() {
                for (auto& testcase : _testcases) {
                    Path input = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    ReturnState state = _results[input];
                    if (!__is_pass(expected, state)) return false;
                }
                return true;
            }

            void __short_summary(_msg::OutStream& out) {
                std::vector<Path> fail_files;
                for (auto& testcase : _testcases) {
                    Path input = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    ReturnState state = _results[input];
                    if (!__is_pass(expected, state)) fail_files.push_back(input.full_path());
                }
                if (fail_files.empty()) __all_pass(out);
                else __meets_error_files(out, fail_files, _testcases.size());
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_titles({"TestCase", "Expected", "State", "Pass Test", "Log Message"});
                int count = 0;
                std::vector<Path> fail_files;
                for (auto& testcase : _testcases) {
                    count++;
                    Path input = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    ReturnState state = _results[input];
                    table.add_cell(0, count, tools::string_format("%s ", input.full_path().cname()));
                    table.add_cell(1, count, __expected_msg(expected));
                    table.add_cell(2, count, __state_msg(state));
                    bool is_pass = __is_pass(expected, state);
                    table.add_cell(3, count, is_pass ? _pass_msg : _fail_msg);
                    if (!is_pass) fail_files.push_back(input.full_path());
                    Path log = __log_name(input);
                    table.add_cell(4, count, __get_file_message(log));
                }
                table.draw();
                if (!fail_files.empty()) {
                    __meets_error_files(out, fail_files);
                }
            }
            
        };
    } // namespace io
} // namespace generator

#endif // !_SGPCET_TEST_VALIDATOR_H_
