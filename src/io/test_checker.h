#ifndef _SGPCET_TEST_CHECKER_H_
#define _SGPCET_TEST_CHECKER_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_
#ifndef _SGPCET_CHECKER_H_
#include "checker.h"
#endif // _SGPCET_CHECKER_H_

namespace generator {
    namespace io {
        class _TestChecker : public _Reporter {
        protected:
            struct TestCase {
                Path input;
                Path output;
                Path answer;
            };
            using ExpectedResults = std::map<TestCase*, _enum::ExpectedResult>;
            using TestResults = std::map<TestCase*, _enum::_JudgeState>;
            _Checker _checker;
            int _time_limit;
            ExpectedResults _testcases;
            TestResults _results;
        public:
            _TestChecker() : _checker(), _time_limit(_setting::time_limit_inf) {}

            template<typename T>
            _TestChecker(T&& checker) : _checker(std::forward<T>(checker)), _time_limit(_setting::time_limit_inf) {}

            ~_TestChecker() {
                for (auto& testcase : _testcases) delete testcase.first;
            }

            _GET_VALUE(_Checker, checker);
            _SET_GET_VALUE(int, time_limit);
            _SET_GET_VALUE(ExpectedResults, testcases);
            _GET_VALUE(TestResults, results);

            template<typename T>
            void __set_checker(T&& checker) {
                _checker.__set_checker(std::forward<T>(checker));
            }

            template<typename T1, typename T2, typename T3>
            void __add_testcase(T1&& input, T2&& output, T3&& answer, _enum::ExpectedResult result = _enum::ExpectedResult::Any) {
                TestCase* testcase = new TestCase({Path(std::forward<T1>(input)), Path(std::forward<T2>(output)), Path(std::forward<T3>(answer))});
                _testcases[testcase] = result;
            }

            bool __empty() {
                return _testcases.empty();
            }

            std::string __testcase_combine_name(TestCase* testcase) {
                return testcase->input.__file_name() + "_" + testcase->output.__file_name() + "_" + testcase->answer.__file_name();
            }

            Path __testlib_log_name(TestCase* testcase) {
                std::string combine = __testcase_combine_name(testcase);
                return __path_join(__test_checker_folder(), __end_with(combine, _enum::_CHECK_RESULT));
            }

            void __run() {
                Path folder = __test_checker_folder();
                __create_directories(folder);
                _checker.set_time_limit(_time_limit);
                int count = 1;
                for (auto& testcase : _testcases) {
                    _msg::__flash_msg(_msg::_defl, "Test Checker : ", __ratio_msg(count, _testcases.size()));
                    TestCase* files = testcase.first;
                    Path input = files->input;
                    Path output = files->output;
                    Path answer = files->answer;
                    Path log = __testlib_log_name(files);
                    _checker.__change_case(input, output, answer, log);
                    _checker.__check_result();
                    _results[files] = _checker.result();
                    count++;
                }
                _msg::__endl(_msg::_defl);
            }

            _msg::_ColorMsg __state_msg(_enum::_JudgeState result) {
                if (_enum::__is_run_error(result)) return _msg::_ColorMsg("Checker ERROR");
                if (_enum::__is_ac(result)) return _ac_msg;
                if (_enum::__is_wa(result)) return _wa_msg;
                if (_enum::__is_checker_tle(result)) return _msg::_ColorMsg("Checker TLE");
                return _unknown_msg;
            }

            _msg::_ColorMsg __expected_msg(_enum::ExpectedResult result) {
                if (result == _enum::ExpectedResult::Any) return _msg::_ColorMsg("Any");
                if (result == _enum::ExpectedResult::Pass) return _ac_msg;
                if (result == _enum::ExpectedResult::Fail) return _wa_msg;
                if (result == _enum::ExpectedResult::TLE) return _tle_msg;
                return _unknown_msg;
            }

            bool __is_pass(_enum::ExpectedResult expected, _enum::_JudgeState result) {
                if (expected == _enum::ExpectedResult::Any) return true;
                if (expected == _enum::ExpectedResult::TLE) return result == _enum::_JudgeState::_CHECKER_TLE;
                if (expected == _enum::ExpectedResult::Pass) return result == _enum::_JudgeState::_AC;
                if (expected == _enum::ExpectedResult::Fail) return result == _enum::_JudgeState::_WA;
                return false;
            }

            bool __is_all_pass() {
                for (auto& testcase : _testcases) {
                    TestCase* files = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    _enum::_JudgeState result = _results[files];
                    if (!__is_pass(expected, result)) return false;
                }
                return true;
            }

            void __meets_error_files(_msg::OutStream& out, std::vector<TestCase*>& files, bool detail) {
                int total = _testcases.size();
                __pass_ratio_msg(out, total - files.size(), total);
                __meets_error_endl(out);
                if (detail) {
                    for (TestCase* testcase : files) {
                        out.println(tools::string_format(
                            "  {%s\n %s\n %s},", 
                            testcase->input.full_path().cname(), 
                            testcase->output.full_path().cname(), 
                            testcase->answer.full_path().cname()));
                    }
                } else {
                    std::vector<std::string> short_names;
                    for (TestCase* testcase : files) {
                        short_names.push_back(__testcase_combine_name(testcase));
                    }
                    std::string file = join(short_names, ", ");
                    out.println(tools::string_format(" {%s}", file.c_str()));
                }

            }

            void __short_summary(_msg::OutStream& out) {
                std::vector<TestCase*> fail_testcases;
                for (auto& testcase : _testcases) {
                    TestCase* files = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    _enum::_JudgeState result = _results[files];
                    if (!__is_pass(expected, result)) fail_testcases.push_back(files);
                }
                if (fail_testcases.empty()) __all_pass(out);
                else __meets_error_files(out, fail_testcases, false);
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_titles({"ShortName", "Expected", "State", "Pass Test", "Log Message"});
                int count = 0;
                std::vector<TestCase*> fail_files;
                for (auto& testcase : _testcases) {
                    count++;
                    TestCase* files = testcase.first;
                    _enum::ExpectedResult expected = testcase.second;
                    _enum::_JudgeState result = _results[files];
                    table.add_cell(0, count, __testcase_combine_name(files));
                    table.add_cell(1, count, __expected_msg(expected));
                    table.add_cell(2, count, __state_msg(result));
                    bool is_pass = __is_pass(expected, result);
                    table.add_cell(3, count, is_pass ? _pass_msg : _fail_msg);
                    if (!is_pass) fail_files.push_back(files);
                    Path log = __testlib_log_name(files);
                    table.add_cell(4, count, __get_file_message(log));
                }
                table.draw();
                if (!fail_files.empty()) {
                    __meets_error_files(out, fail_files, true);
                }
            }
            
        };
    } // namespace io
} // namespace generator

#endif // !_SGPCET_TEST_CHECKER_H_
