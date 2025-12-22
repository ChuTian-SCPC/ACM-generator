#ifndef _SGPCET_COMPARE_H_
#define _SGPCET_COMPARE_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_
#ifndef _SGPCET_CHECKER_H_
#include "checker.h"
#endif // _SGPCET_CHECKER_H_

namespace generator {
    namespace io {
        class _Compare : public _Reporter {
        protected:
            using Name = std::string;
            using TestCase = std::pair<Name, int>;
            using TestResult = std::pair<_enum::_JudgeState, int>;
            _Checker _checker;
            int _time_limit;
            int _time_limit_for_checker;
            std::map<Name, _Program*> _name;
            std::map<Name, std::set<int>> _testcases;
            std::map<TestCase, TestResult> _results;
        public:
            _Compare() : _checker(), _time_limit(_setting::time_limit_inf), _time_limit_for_checker(_setting::time_limit_inf) {}    

            template<typename T>
            _Compare(T&& checker, int time_limit) : _checker(std::forward<T>(checker)), _time_limit(time_limit), _time_limit_for_checker(_setting::time_limit_inf) {}

            template<typename T>
            void __set_checker(T&& checker) {
                _checker.__set_checker(std::forward<T>(checker));
            }

            _SET_GET_VALUE(int, time_limit)
            _SET_GET_VALUE(int, time_limit_for_checker)

            ~_Compare() {
                for (auto& name : _name) {
                    if (name.second != nullptr) delete name.second;
                }
            }

            template<typename T>
            void __add_testcase(T&& program, const std::vector<int>& cases) {
                _Program* p = __program(std::forward<T>(program));
                std::string name = p->name();
                if (_name.find(name) == _name.end()) _name[name] = p;
                else delete p;
                for (auto& case_id : cases) {
                    if (__testcase_input_file_exists(case_id) && __testcase_output_file_exists(case_id))
                        _testcases[name].insert(case_id);
                }
            }

            template<typename T>
            void __add_testcase(T&& program, int start, int end) {
                std::vector<int> cases;
                for (int i = start; i <= end; i++) cases.push_back(i);
                __add_testcase(std::forward<T>(program), cases);
            }

            template<typename T>
            void __add_testcase(T&& program) {
                __add_testcase(std::forward<T>(program), __get_all_inputs());
            }

            bool __enable_judge_ans(int runtime, int time_limit, _enum::_JudgeState& result) {
                if (!__time_limit_exceed(runtime, time_limit)) return true;
                result = _enum::_JudgeState::_TLE;
                return runtime <= time_limit * _setting::time_limit_check_ratio;
            }

            void __run_case(Name name) {
                Path ans_folder = __path_join(__compare_folder(), name);
                __create_directories(ans_folder);
                int count = 1;
                int sum = _testcases[name].size();
                _Program* p = _name[name];
                if (!p->__check_program_valid())  return;
                for (auto& case_id : _testcases[name]) {
                    TestCase test_case(name, case_id);
                    TestResult& result = _results[test_case];
                    _msg::__flash_msg(_msg::_defl, tools::string_format("Compare program %s : ", name.c_str()), __ratio_msg(count, sum));
                    count++;
                    Path input = __testcase_input_file_path(case_id);
                    Path output = __testcase_output_file_path(case_id);
                    Path answer = __path_join(ans_folder, __end_with(case_id, _enum::_ANS));
                    Path check_result = __path_join(ans_folder, __end_with(case_id, _enum::_CHECK_RESULT));
                    auto res = p->__run_program(input, answer, _setting::_default_path, __time_limit_extend(_time_limit), _enum::_FuncProgramType::_RESULT);
                    
                    if (!__is_success(res.exit_code)) {
                        result.first = _enum::_JudgeState::_ERROR;
                        continue;
                    }
                    result.second = res.time;
                    if (__enable_judge_ans(res.time, _time_limit, result.first)) {
                        _checker.__change_case(input, output, answer, check_result);
                        _checker.__check_result();
                        _enum::_JudgeState check_result = _checker.result();
                        if (_enum::__is_run_error(check_result)) result.first = check_result;
                        else result.first |= check_result;
                    }
                }
                _msg::__endl(_msg::_defl);
            }

            void __compare() {
                if (_testcases.empty()) return;
                _checker.set_time_limit(_time_limit_for_checker);
                _results.clear();
                for (auto& name : _name) {
                    for (auto& case_id : _testcases[name.first]) 
                        _results[{name.first, case_id}] = {_enum::_JudgeState::_UNKNOWN, 0};
                    __run_case(name.first);
                }
            }

            int __get_total_success(Name name) {
                int total = 0;
                for (auto& case_id : _testcases[name]) {
                    TestCase test_case(name, case_id);
                    auto result = _results[test_case].first;
                    if (_enum::__is_consider_state(result)) total++;
                }
                return total;
            }

            TestResult __get_test_result(Name name) {
                TestResult total = {_enum::_JudgeState::_UNKNOWN, 0};
                for (auto& case_id : _testcases[name]) {
                    TestCase test_case(name, case_id);
                    auto result = _results[test_case].first;
                    auto time = _results[test_case].second;
                    if (_judge_result_priority[__state_index(result)] > _judge_result_priority[__state_index(total.first)]) 
                        total.first = result;
                    if (_enum::__is_consider_state(result)) 
                        total.second = std::max(total.second, time);
                    
                }
                return total;
            }

            void __push_state(_Table& table, int row, int col, _enum::_JudgeState state) {
                table.add_msg(row, col, __state_msg(state, true));
                if (_enum::__is_combine_state(state)) {
                    table.add_msg(row, col, "(");
                    table.add_msg(row, col, __state_msg(state, false));
                    table.add_msg(row, col, ")");
                }
            }

            void __push_time(_Table& table, int row, int col, TestResult result, std::string space = "") {
                if (_enum::__is_consider_state(result.first)) {
                    table.add_msg(row, col, space);
                    table.add_msg(row, col, tools::string_format("%dms", result.second));
                }   
            }

            void __short_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_titles({"Program Name", "Run Success", "State", "RunTime"});
                int count = 1;
                for (auto& testcase : _name) {
                    Name name = testcase.first;
                    TestResult result = __get_test_result(name);
                    int success = __get_total_success(name);
                    int sum = _testcases[name].size();
                    table.add_cell(0, count, name);
                    table.add_cell(1, count, __ratio_msg(success, sum));
                    __push_state(table, 2, count, result.first);
                    __push_time(table, 3, count, result);
                    count++;
                }
                table.draw();
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_cell(0, 0, "Case \\ Name");
                std::set<int> all_case_ids;
                for (auto& testcase : _name) {
                    for (auto& case_id : _testcases[testcase.first]) {
                        all_case_ids.insert(case_id);
                    }
                }
                int column = 1;
                for (auto& case_id : all_case_ids) {
                    table.add_cell(0, column, std::to_string(case_id));
                    column++;
                }
                table.add_cell(0, column, "Total");
                int row = 1;
                for (auto& testcase : _name) {
                    Name name = testcase.first;
                    TestResult total_result = __get_test_result(name);
                    table.add_cell(row, 0, name);
                    int len = 1;
                    for (auto& case_id : all_case_ids) {
                        TestCase test_case(name, case_id);
                        if (_results.find(test_case) == _results.end()) {
                            table.add_cell(row, len, "N/A");
                        } else {
                            TestResult result = _results[test_case];
                            __push_state(table, row, len, result.first);
                            __push_time(table, row, len, result, " ");
                        }
                        len++;
                    }
                    __push_state(table, row, len, total_result.first);
                    __push_time(table, row, len, total_result, " ");
                    row++;
                }
                table.draw();
            }

            bool __empty() {
                return _name.empty();
            }
                int __size() {
                return _name.size();
            }
        };

        void __add_compare_program(_Compare& , int , int ) {
            return;
        }

        void __add_compare_program(_Compare&) {
            return;
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __add_compare_program(_Compare& compare, int start, int end, T first, Args... args) {
            compare.__add_testcase(first, start, end);
            __add_compare_program(compare, start, end, args...);
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __add_compare_program(_Compare& compare, T first, Args... args) {
            compare.__add_testcase(first);
            __add_compare_program(compare, args...);
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int start, int end, int time_limit, T&& checker, Args... args) {
            _Compare compare(std::forward<T>(checker), time_limit);
            __add_compare_program(compare, start, end, args...);
            compare.__compare();
            compare.__detail_summary(_msg::_defl);
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int time_limit, T&& checker, Args... args) {
            _Compare compare(std::forward<T>(checker), time_limit);
            __add_compare_program(compare, args...);
            compare.__compare();
            compare.__detail_summary(_msg::_defl);
        }
    } // namespace io
} // namespace generator

#endif // _SGPCET_COMPARE_H_
