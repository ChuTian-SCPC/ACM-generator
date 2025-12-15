#ifndef _SGPCET_HACK_H_
#define _SGPCET_HACK_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_
#ifndef _SGPCET_CHECKER_H_
#include "checker.h"
#endif // _SGPCET_CHECKER_H_

namespace generator {
    namespace io {
        class _Hack : public _Reporter {
        protected:
            _Checker _checker;
            _Program* _generator;
            _Program* _std;
            _Program* _compare;
            _Program* _validator;
            int _time_limit;
            bool _limit_std_runtime;
            int _max_try;
            bool _stop_when_wrong;
            bool _copy_wrong_to_testcase;
            bool _delete_correct;
            int _start_index;
            int _try_count;

            enum class State {
                UNKNOWN,
                AC,
                WA,
                TLE,
                RE,
                STD_TLE,
                STD_RE,
                GEN_FAIL,
                VAL_FAIL,
                CHECK_FAIL
            };
            using TestResult = std::pair<State, int>;
            std::vector<TestResult> _states;
            std::map<int, int> _move_path;

        public:

            template<typename T1, typename T2, typename T3, typename T4>
            _Hack(T1&& generator, T2&& std, T3&& compare, T4&& checker,
                int time_limit, bool limit_std_runtime, int max_try,
                bool stop_when_wrong, bool copy_wrong_to_testcase,
                bool delete_correct)
                : _checker(std::forward<T4>(checker)), 
                _generator(nullptr), _std(nullptr), 
                _compare(nullptr), _validator(nullptr),
                _time_limit(time_limit),
                _limit_std_runtime(limit_std_runtime), _max_try(max_try),
                _stop_when_wrong(stop_when_wrong),
                _copy_wrong_to_testcase(copy_wrong_to_testcase),
                _delete_correct(delete_correct), _start_index(1) {
                    __set_generator(std::forward<T1>(generator));
                    __set_std(std::forward<T2>(std));
                    __set_compare(std::forward<T3>(compare));

                }

            ~_Hack() {
                if (_generator != nullptr) delete _generator;
                if (_std != nullptr) delete _std;
                if (_compare != nullptr) delete _compare;
            }

            _SET_GET_VALUE(int, time_limit);
            _SET_GET_VALUE(int, max_try);
            _SET_GET_VALUE(bool, stop_when_wrong);
            _SET_GET_VALUE(bool, copy_wrong_to_testcase);
            _SET_GET_VALUE(bool, delete_correct);
            _SET_GET_VALUE(int, start_index);

            template<typename T>
            void __set_generator(T&& generator) {
                if (_generator != nullptr) delete _generator;
                _generator = __program(generator);
            }

            template<typename T>
            void __set_std(T&& std) {
                if (_std != nullptr) delete _std;
                _std = __result_program(std);
            }

            template<typename T>
            void __set_compare(T&& compare) {
                if (_compare != nullptr) delete _compare;
                _compare = __result_program(compare);
            }

            template<typename T>
            void __set_validator(T&& validator) {
                if (_validator != nullptr) delete _validator;
                _validator = __result_program(validator);
            }

            State __generate(int index) {
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                _Program* gen = __generator_program(_generator, index, true);
                ReturnState result = gen->__run_program(_setting::_default_path, input, _setting::_default_path, _setting::time_limit_inf, _enum::_FuncProgramType::_GENERATOR);
                delete gen;
                if (!__is_success(result.exit_code)) return State::GEN_FAIL;
                return State::UNKNOWN;
            }

            State __validate(int index) {
                if (_validator == nullptr) return State::UNKNOWN;
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                Path log = __path_join(__hack_folder(), __end_with(index, _enum::_VAL));
                ReturnState result = _validator->__run_program(input, _setting::_default_path, _setting::_default_path, _setting::time_limit_inf, _enum::_FuncProgramType::_VALIDATOR);
                if (!__is_success(result.exit_code)) return State::VAL_FAIL;
                return State::UNKNOWN;
            }

            State __run_std(int index) {
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__hack_folder(), __end_with(index, _enum::_OUT));
                int std_time_limit = _limit_std_runtime ? _time_limit : _setting::time_limit_inf;
                ReturnState result = _std->__run_program(input, output, _setting::_default_path, std_time_limit, _enum::_FuncProgramType::_RESULT);
                if (!__is_success(result.exit_code)) return State::STD_RE;
                if (__time_limit_exceed(result.time, _time_limit)) return State::STD_TLE;
                return State::UNKNOWN;
            }

            TestResult __run_user(int index) {
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                Path user = __path_join(__hack_folder(), __end_with(index, _enum::_ANS));
                ReturnState result = _compare->__run_program(input, user, _setting::_default_path, _time_limit, _enum::_FuncProgramType::_RESULT);
                if (!__is_success(result.exit_code)) return {State::RE, 0};
                if (__time_limit_exceed(result.time, _time_limit)) return {State::TLE, result.time};
                return {State::UNKNOWN, result.time};
            }

            State __check(int index) {
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__hack_folder(), __end_with(index, _enum::_OUT));
                Path user = __path_join(__hack_folder(), __end_with(index, _enum::_ANS));
                Path check_log = __path_join(__hack_folder(), __end_with(index, _enum::_CHECK_RESULT));
                _checker.__change_case(input, output, user, check_log);
                _checker.__check_result();
                _enum::_JudgeState result = _checker.result();
                if (result == _enum::_JudgeState::_AC) return State::AC;
                if (result == _enum::_JudgeState::_WA) return State::WA;
                return State::CHECK_FAIL;
            }

            bool __is_user_correct(State& state) {
                return state == State::AC;
            }

            bool __is_user_uncorrect(State& state) {
                return state == State::WA ||
                    state == State::RE ||
                    state == State::TLE;
            };

            bool __is_program_error(State& state) {
                return state != State::AC &&
                    state != State::UNKNOWN &&
                    !__is_user_uncorrect(state);
            }

            bool __has_runtime(State& state) {
                return state == State::AC ||
                    state == State::WA ||
                    state == State::TLE;
            }

            int __find_next_not_exist_inputs(int start) {
                for (int i = start; ; i++) {
                    if (i > _setting::test_case_limit) return -1;
                    if (!__testcase_input_file_exists(i)) return i;
                }
            }

            void __copy_uncorrect_case() {
                if (!_copy_wrong_to_testcase) return;
                __create_directories(__testcases_folder());
                _move_path.clear();
                int next_input = 1;
                for (int i = 0; i < std::min(_max_try, _try_count); i++) {
                    if (!__is_user_uncorrect(_states[i].first)) continue;
                    int index = i + _start_index;
                    Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                    Path output = __path_join(__hack_folder(), __end_with(index, _enum::_OUT));
                    next_input = __find_next_not_exist_inputs(next_input);
                    if (next_input == -1) {
                        _msg::__warn_msg(_msg::_defl, tools::string_format("Testcases are over the test_case_limit(%d).", _setting::test_case_limit));
                        break;
                    }
                    _move_path[i] = next_input;
                    Path testcase_input = __testcase_input_file_path(next_input);
                    Path testcase_output = __testcase_output_file_path(next_input);
                    __copy_file(input, testcase_input);
                    __copy_file(output, testcase_output);
                }
            }

            void __delete_files(int i) {
                if (!_delete_correct) return ;
                if (_states[i].first != State::AC) return;
                int index = i + _start_index;
                Path input = __path_join(__hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__hack_folder(), __end_with(index, _enum::_OUT));
                Path user = __path_join(__hack_folder(), __end_with(index, _enum::_ANS));
                Path check_log = __path_join(__hack_folder(), __end_with(index, _enum::_CHECK_RESULT));
                Path val_log = __path_join(__hack_folder(), __end_with(index, _enum::_VAL));
                input.__delete_file();
                output.__delete_file();
                user.__delete_file();
                check_log.__delete_file();
                val_log.__delete_file();
            }

            void __hack() {
                if (_max_try <= 0) _msg::__fail_msg(_msg::_defl, "At least try once.");
                __create_directories(__hack_folder());
                _states.clear();
                _states.resize(_max_try, {State::UNKNOWN, 0});
                _try_count = 1;
                for (int i = 0; i < _max_try; i++, _try_count++) {
                    int index = i + _start_index;
                    Path input = __path_join(__hack_folder(), __end_with(i, _enum::_IN));
                    Path output = __path_join(__hack_folder(), __end_with(i, _enum::_OUT));
                    Path user = __path_join(__hack_folder(), __end_with(i, _enum::_ANS));
                    _msg::__flash_msg(_msg::_defl, "Hack : ", __ratio_msg(i + 1, _max_try));
                    State& state = _states[i].first;
                    state = __generate(index);
                    if (state != State::UNKNOWN) continue;
                    state = __validate(index);
                    if (state != State::UNKNOWN) continue;
                    state = __run_std(index);
                    if (state != State::UNKNOWN) continue;
                    _states[i] = __run_user(index);
                    if (state != State::UNKNOWN) continue;
                    state = __check(index);
                    __delete_files(i);
                    if (__is_user_uncorrect(state) && _stop_when_wrong) break;
                }
                __copy_uncorrect_case();
                _msg::__endl(_msg::_defl);
            }

            void __short_summary(_msg::OutStream& out) {
                int passed = 0;
                int program_error = 0;
                int sum = std::min(_max_try, _try_count);
                for (int i = 0; i < sum; i++) {
                    if (__is_user_correct(_states[i].first)) passed++;
                    else if (__is_program_error(_states[i].first)) program_error++;
                }
                if (passed == sum) __all_pass(out);
                else {
                    out.print(_fail_msg, " : ", __ratio_msg(sum - passed - program_error, sum, ", "));
                    out.println(_error_msg, " : ", __ratio_msg(program_error, sum));
                }
            }

            _msg::_ColorMsg __state_msg(State state) {
                switch (state) {
                    case State::AC: return _ac_msg;
                    case State::WA: return _wa_msg;
                    case State::RE: return _run_error_msg;
                    case State::TLE: return _tle_msg;
                    case State::STD_TLE: return _msg::_ColorMsg("std TLE", _enum::Color::Yellow);
                    case State::STD_RE: return _msg::_ColorMsg("std RE", _enum::Color::Red);
                    case State::GEN_FAIL: return _msg::_ColorMsg("gen ERROR", _enum::Color::Red);
                    case State::VAL_FAIL: return _msg::_ColorMsg("validate ERROR", _enum::Color::Red);
                    case State::CHECK_FAIL: return _msg::_ColorMsg("check ERROR", _enum::Color::Red);
                    default: return _msg::_ColorMsg("UNKNOWN", _enum::Color::Red);
                }
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_titles({"Hack Case Id", "State", "RunTime"});
                if (_copy_wrong_to_testcase && _move_path.size() > 0) table.add_cell(3, 0, "Move Path");
                for (int i = 0; i < std::min(_max_try, _try_count); i++) {
                    int index = i + _start_index;
                    table.add_cell(0, i + 1, std::to_string(index));
                    table.add_cell(1, i + 1, __state_msg(_states[i].first));
                    if (__has_runtime(_states[i].first)) table.add_cell(2, i + 1, tools::string_format("%dms", _states[i].second));
                    if (_copy_wrong_to_testcase && _move_path.find(i) != _move_path.end()) 
                        table.add_cell(3, i + 1, __testcase_input_file_path(_move_path[i]).path());
                }
                table.draw();
            }
        };

        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgramConstructible<T1>::value &&
            IsProgramConstructible<T2>::value &&
            IsProgramConstructible<T3>::value &&
            IsCheckerConstructible<T4>::value
        >::type
        hack(T1&& generator_program, T2&& std_program, 
            T3&& compare_program, T4&& checker_program, 
            int time_limit, bool limit_std_runtime = false,
            int max_try = 100, bool stop_when_wrong = true, 
            bool copy_wrong_to_testcase = true, bool delete_correct = true) {
                _Hack hack(std::forward<T1>(generator_program), std::forward<T2>(std_program), 
                    std::forward<T3>(compare_program), std::forward<T4>(checker_program), 
                    time_limit, limit_std_runtime, max_try, stop_when_wrong, copy_wrong_to_testcase, delete_correct);
                hack.__hack();
                hack.__detail_summary(_msg::_defl);
            }
    } // namespace io
} // namespace generator

#endif // _SGPCET_HACK_H_
