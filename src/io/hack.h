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
            using Name = std::string;
            struct _HackSetting {
                int _max_try;
                int _start_index;
                bool _stop_when_wrong;
                bool _should_skip;
                
                _HackSetting(int max_try, int start_index, bool stop_when_wrong)
                    : _max_try(max_try), _start_index(start_index), _stop_when_wrong(stop_when_wrong), _should_skip(false) {}
            };
            struct _Comparer {
                _Program* _program;
                std::vector<_HackSetting> _cases;
                int _last_index;
                std::set<int> _indices;

                _Comparer(_Program* program = nullptr)
                    : _program(program), _last_index(1) {}

                ~_Comparer() {
                    if (_program != nullptr)
                        delete _program;
                }

                _Comparer(const _Comparer& other)
                    : _program(other._program ? other._program->__clone() : nullptr),
                    _cases(other._cases),
                    _last_index(other._last_index),
                    _indices(other._indices) {}
    
                _Comparer& operator=(const _Comparer& other) {
                    if (this != &other) {
                        delete _program;
                        _program = other._program ? other._program->__clone() : nullptr;
                        _cases = other._cases;
                        _last_index = other._last_index;
                        _indices = other._indices;
                    }
                    return *this;
                }
    
                _Comparer(_Comparer&& other) noexcept
                    : _program(other._program),
                    _cases(std::move(other._cases)),
                    _last_index(other._last_index),
                    _indices(std::move(other._indices)) {
                    other._program = nullptr;
                }
    
                _Comparer& operator=(_Comparer&& other) noexcept {
                    if (this != &other) {
                        delete _program;
                        _program = other._program;
                        other._program = nullptr;
                        _cases = std::move(other._cases);
                        _last_index = other._last_index;
                        _indices = std::move(other._indices);
                    }
                    return *this;
                }

                void set_program(_Program* program) {
                    if (_program != nullptr) delete _program;
                    _program = program;
                }

                void add_cases(int max_try, int start_index, bool stop_when_wrong) {
                    _last_index = std::max(_last_index, start_index + max_try);
                    _cases.push_back(_HackSetting(max_try, start_index, stop_when_wrong));
                    for (int i = start_index; i < start_index + max_try; i++)
                        _indices.insert(i);
                }
            };
            std::map<Name, _Comparer> _comparers;
            _Checker _checker;
            _Program* _generator;
            _Program* _std;

            _Program* _validator;
            int _time_limit;
            int _time_limit_for_std;
            int _time_limit_for_generator;
            int _time_limit_for_checker;
            int _time_limit_for_validator;

            bool _copy_wrong_to_testcase;
            bool _delete_correct;

            std::string _sub_folder_name;

            enum class State {
                UNKNOWN,
                SKIP,
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
            using TestCase = std::pair<Name, int>;
            std::map<TestCase, TestResult> _states;
            std::map<int, int> _move_path;
            std::map<int, std::set<Name>> _testcases;

        public:
            _Hack() : _checker(), _generator(nullptr), _std(nullptr), _validator(nullptr),
                _time_limit(_setting::time_limit_inf), _time_limit_for_std(_setting::time_limit_inf),
                _time_limit_for_generator(_setting::time_limit_inf), _time_limit_for_checker(_setting::time_limit_inf),
                _time_limit_for_validator(_setting::time_limit_inf), _copy_wrong_to_testcase(false), _delete_correct(false) {}

            template<typename T1, typename T2, typename T3>
            _Hack(T1&& generator, T2&& std, T3&& checker,
                int time_limit, int time_limit_for_std, bool copy_wrong_to_testcase, bool delete_correct)
                : _checker(std::forward<T3>(checker)), 
                _generator(nullptr), _std(nullptr), _validator(nullptr),
                _time_limit(time_limit),
                _time_limit_for_std(time_limit_for_std),
                _time_limit_for_generator(_setting::time_limit_inf),
                _time_limit_for_checker(_setting::time_limit_inf),
                _time_limit_for_validator(_setting::time_limit_inf),
                _copy_wrong_to_testcase(copy_wrong_to_testcase),
                _delete_correct(delete_correct),
                _sub_folder_name(_setting::_empty_program_name) {
                    __set_generator(std::forward<T1>(generator));
                    __set_std(std::forward<T2>(std));
                }

            ~_Hack() {
                if (_generator != nullptr) delete _generator;
                if (_std != nullptr) delete _std;
            }

            _GET_VALUE(_Checker, checker);
            _GET_VALUE(_Program*, generator);
            _GET_VALUE(_Program*, std);
            _GET_VALUE(_Program*, validator);
            _SET_GET_VALUE(int, time_limit);
            _SET_GET_VALUE(int, time_limit_for_std);
            _SET_GET_VALUE(int, time_limit_for_generator);
            _SET_GET_VALUE(int, time_limit_for_checker);
            _SET_GET_VALUE(int, time_limit_for_validator);
            _SET_GET_VALUE(bool, copy_wrong_to_testcase);
            _SET_GET_VALUE(bool, delete_correct);
            _SET_GET_VALUE(std::string, sub_folder_name);

            template<typename T>
            void __set_checker(T&& checker) {
                _checker.__set_checker(std::forward<T>(checker));
            }

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
            void __add_comparer(T&& comparer, int max_try, int start_index, bool stop_when_wrong) {
                if (max_try <= 0) _msg::__fail_msg(_msg::_defl, "At least try once.");
                _Program* program = __result_program(comparer);
                Name name = program->name();
                if (_comparers.find(name) == _comparers.end()) _comparers[name] = _Comparer(program);
                else delete program;
                _comparers[name].add_cases(max_try, start_index, stop_when_wrong);
            }

            template<typename T>
            void __add_comparer(T&& comparer, int max_try, bool stop_when_wrong) {
                if (max_try <= 0) _msg::__fail_msg(_msg::_defl, "At least try once.");
                _Program* program = __result_program(comparer);
                Name name = program->name();
                if (_comparers.find(name) == _comparers.end()) _comparers[name] = _Comparer(program);
                else delete program;
                int last_index = _comparers[name]._last_index;
                _comparers[name].add_cases(max_try, last_index, stop_when_wrong);
            }

            template<typename T>
            void __set_validator(T&& validator) {
                if (_validator != nullptr) delete _validator;
                _validator = __result_program(validator);
            }

            Path __case_hack_folder() {
                if (_sub_folder_name == _setting::_empty_program_name) return __hack_folder();
                return __path_join(__hack_folder(), _sub_folder_name);
            }

            State __generate(int index) {
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                _Program* gen = __generator_program(_generator, index, true);
                ReturnState result = gen->__run_program(_setting::_default_path, input, _setting::_default_path, _time_limit_for_generator, _enum::_FuncProgramType::_GENERATOR);
                delete gen;
                if (!__is_success(result.exit_code)) return State::GEN_FAIL;
                return State::UNKNOWN;
            }

            State __validate(int index) {
                if (_validator == nullptr) return State::UNKNOWN;
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                Path log = __path_join(__case_hack_folder(), __end_with(index, _enum::_VAL));
                ReturnState result = _validator->__run_program(input, _setting::_default_path, log, _time_limit_for_validator, _enum::_FuncProgramType::_VALIDATOR);
                if (!__is_success(result.exit_code)) return State::VAL_FAIL;
                return State::UNKNOWN;
            }

            State __run_std(int index) {
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__case_hack_folder(), __end_with(index, _enum::_OUT));
                ReturnState result = _std->__run_program(input, output, _setting::_default_path, _time_limit_for_std, _enum::_FuncProgramType::_RESULT);
                if (!__is_success(result.exit_code)) return State::STD_RE;
                if (__time_limit_exceed(result.time, _time_limit)) return State::STD_TLE;
                return State::UNKNOWN;
            }
            
            void __prepare_comparers() {
                _states.clear();
                _testcases.clear();
                for (auto& comparer : _comparers) {
                    Name name = comparer.first;
                    for (auto& index : comparer.second._indices) {
                        {
                            _states[TestCase(name, index)] = TestResult(State::UNKNOWN, 0);
                            _testcases[index].insert(name);
                        }
                    }
                }
            }

            bool __should_skip(const _HackSetting& setting, int index) {
                if (setting._should_skip) return true;
                if (index < setting._start_index || index >= setting._start_index + setting._max_try) return true;
                return false;
            }

            bool __should_skip(const _Comparer& comparer, int index) {
                bool skip = true;
                for (auto& setting : comparer._cases) {
                    skip &= __should_skip(setting, index);
                }
                return skip;
            }
            
            bool __should_skip(const Name& name, int index) {
                return __should_skip(_comparers[name], index);
            }

            bool __should_skip(int index) {
                bool skip = true;
                for (auto& name : _testcases[index]) {
                    skip &= __should_skip(_comparers[name], index);
                }
                return skip;
            }

            void __stop_hack(const Name& name, int index) {
                for (auto& testcase : _comparers[name]._cases) {
                    if (!testcase._stop_when_wrong) continue; 
                    if (index < testcase._start_index || index >= testcase._start_index + testcase._max_try) continue;
                    testcase._should_skip = true;
                    auto& state = _states[TestCase(name, index)];
                }
            }

            TestResult __run_user(const Name& name, int index) {
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                Path user = __path_join(__case_hack_folder(), name, __end_with(index, _enum::_ANS));
                ReturnState result = _comparers[name]._program->__run_program(input, user, _setting::_default_path, _time_limit, _enum::_FuncProgramType::_RESULT);
                if (!__is_success(result.exit_code)) return {State::RE, 0};
                if (__time_limit_exceed(result.time, _time_limit)) return {State::TLE, result.time};
                return {State::UNKNOWN, result.time};
            }

            State __check(const Name& name, int index) {
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__case_hack_folder(), __end_with(index, _enum::_OUT));
                Path user = __path_join(__case_hack_folder(), name, __end_with(index, _enum::_ANS));
                Path check_log = __path_join(__case_hack_folder(), name, __end_with(index, _enum::_CHECK_RESULT));
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

            bool __is_skip(State& state) {
                return state == State::SKIP;
            }

            bool __is_user_uncorrect(State& state) {
                return state == State::WA ||
                    state == State::RE ||
                    state == State::TLE;
            }

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

            void __copy_uncorrect_cases() {
                if (!_copy_wrong_to_testcase) return;
                __create_directories(__testcases_folder());
                _move_path.clear();
                int next_input = 1;
                for (auto& testcase : _testcases) {
                    int index = testcase.first;
                    bool should_copy = false;
                    for (auto& name : testcase.second) {
                        should_copy |= __is_user_uncorrect(_states[TestCase(name, index)].first);
                        if (should_copy) break;
                    }
                    if (!should_copy) continue;
                    Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                    Path output = __path_join(__case_hack_folder(), __end_with(index, _enum::_OUT));
                    next_input = __find_next_not_exist_inputs(next_input);
                    if (next_input == -1) {
                        _msg::__warn_msg(_msg::_defl, tools::string_format("Testcases are over the test_case_limit(%d).", _setting::test_case_limit));
                        break;
                    }
                    _move_path[index] = next_input;
                    Path testcase_input = __testcase_input_file_path(next_input);
                    Path testcase_output = __testcase_output_file_path(next_input);
                    __copy_file(input, testcase_input);
                    __copy_file(output, testcase_output);
                }
            }

            void __delete_files(int index) {
                if (!_delete_correct) return ;
                bool all_correct = true;
                for (auto& name : _testcases[index]) {
                    State& state = _states[TestCase(name, index)].first;
                    if (__is_user_correct(state)) {
                        Path user = __path_join(__case_hack_folder(), name, __end_with(index, _enum::_ANS));
                        Path check_log = __path_join(__case_hack_folder(), name, __end_with(index, _enum::_CHECK_RESULT));
                        user.__delete_file();
                        check_log.__delete_file(); 
                    } else {
                        all_correct = false;
                    }
                }
                if (!all_correct) return;
                Path input = __path_join(__case_hack_folder(), __end_with(index, _enum::_IN));
                Path output = __path_join(__case_hack_folder(), __end_with(index, _enum::_OUT));
                Path val_log = __path_join(__case_hack_folder(), __end_with(index, _enum::_VAL));
                input.__delete_file();
                output.__delete_file();
                val_log.__delete_file();
            }

            void __hack() {
                __prepare_comparers();
                if (_testcases.empty()) return;
                _checker.set_time_limit(_time_limit_for_checker);
                __create_directories(__case_hack_folder());
                int count = 1;
                int sum = _testcases.size();
                for (auto it = _testcases.begin(); it != _testcases.end();) {
                    int index = it->first;
                    if (__should_skip(index)) {
                        for (auto& name : _testcases[index]) {
                            auto& state = _states[TestCase(name, index)].first;
                            if (state == State::UNKNOWN) state = State::SKIP;
                        }
                        it = _testcases.erase(it);
                        continue;
                    }
                    _msg::__flash_msg(_msg::_defl, "Hack : ", __ratio_msg(count, sum));
                    count++;
                    State state = State::UNKNOWN;
                    state = __generate(index);
                    if (state == State::UNKNOWN) state = __validate(index);
                    if (state == State::UNKNOWN) state = __run_std(index);
                    for (auto& name : it->second) {
                        __create_directories(__path_join(__case_hack_folder(), name));
                        TestCase testcase(name, index);
                        if (_states.find(testcase) == _states.end()) continue;
                        if (__should_skip(name, index)) {
                            _states[testcase].first = State::SKIP;
                            continue;
                        }
                        if (state != State::UNKNOWN) {
                            _states[testcase].first = state;
                            continue;
                        }
                        auto& result = _states[testcase];
                        result = __run_user(name, index);
                        if (result.first == State::UNKNOWN) result.first = __check(name, index);
                        if (__is_user_uncorrect(result.first)) __stop_hack(name, index);     
                    }
                    __delete_files(index);
                    it++;
                }
                __copy_uncorrect_cases();
                _msg::__endl(_msg::_defl);
            }

            void __short_summary(_msg::OutStream& out) {
                for (auto& comparer : _comparers) {
                    Name name = comparer.first;
                    int fail = 0;
                    int program_error = 0;
                    int sum = 0;
                    for (auto& index : comparer.second._indices) {
                        State& state = _states[TestCase(name, index)].first;
                        if (__is_skip(state)) continue;
                        if (__is_user_uncorrect(state)) fail++;
                        else if (__is_program_error(state)) program_error++;
                        sum++;
                    }
                    out.print(tools::string_format("Hack %s : ", name.c_str()));
                    if (fail == 0 && program_error == 0) __all_pass(out);
                    else {
                        out.print(_fail_msg, " : ", __ratio_msg(fail, sum, ", "));
                        out.println(_error_msg, " : ", __ratio_msg(program_error, sum));         
                    }
                }
                if (_move_path.size() > 0) {
                    out.println("Move case : ");
                    std::vector<int> from, to;
                    for (auto& move : _move_path) {
                        from.push_back(move.first);
                        to.push_back(move.second);
                    }
                    std::string from_str = join(from, ", ");
                    std::string to_str = join(to, ", ");
                    out.println(tools::string_format("  {%s} -> {%s}", from_str.c_str(), to_str.c_str()));
                }
            }

            _msg::_ColorMsg __state_msg(State state) {
                switch (state) {
                    case State::SKIP: return _msg::_ColorMsg("Skip", _enum::Color::Default);
                    case State::AC: return _ac_msg;
                    case State::WA: return _wa_msg;
                    case State::RE: return _run_error_msg;
                    case State::TLE: return _tle_msg;
                    case State::STD_TLE: return _msg::_ColorMsg("std TLE", _enum::Color::Yellow);
                    case State::STD_RE: return _msg::_ColorMsg("std RE", _enum::Color::Red);
                    case State::GEN_FAIL: return _msg::_ColorMsg("generate ERROR", _enum::Color::Red);
                    case State::VAL_FAIL: return _msg::_ColorMsg("validate ERROR", _enum::Color::Red);
                    case State::CHECK_FAIL: return _msg::_ColorMsg("check ERROR", _enum::Color::Red);
                    default: return _msg::_ColorMsg("UNKNOWN", _enum::Color::Red);
                }
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                _msg::__info_msg(out, tools::string_format("Generator Name : %s", _generator->name().c_str()));
                table.add_cell(0, 0, "Hack Case Id");
                table.add_cell(1, 0, "Seed");
                std::map<Name, int> table_indices;
                int row = 2;
                for (auto& comparer : _comparers) {
                    Name name = comparer.first;
                    table.add_cell(row, 0, name);
                    table_indices[name] = row;
                    row++;
                }
                if (_move_path.size() > 0) table.add_cell(row, 0, "Move Path");
                int case_count = 1;
                for (auto& testcase : _testcases) {
                    int index = testcase.first;
                    table.add_cell(0, case_count, std::to_string(index));
                    _Program* gen = __generator_program(_generator, index, true);
                    table.add_cell(1, case_count, gen->get_argv_without_redirection());
                    delete gen;
                    for(auto& program : _comparers) {
                        Name name = program.first;
                        int table_index = table_indices[name];
                        TestCase case_index(name, index);
                        if (_states.find(case_index) == _states.end()) {
                            table.add_cell(table_index, case_count, "N/A");
                            continue;
                        }
                        TestResult result = _states[case_index];
                        table.add_cell(table_index, case_count, __state_msg(result.first));
                        if (__has_runtime(result.first)) table.add_msg(table_index, case_count, tools::string_format(" %dms", result.second));
                    }
                    if (_move_path.size() > 0) {
                        if (_move_path.find(index) != _move_path.end())
                            table.add_cell(row, case_count, __testcase_input_file_path(_move_path[index]).path());
                    }
                    case_count++;
                }
                table.draw();
            }

            bool __empty() {
                return _comparers.empty();
            }
             int __size() {
                return _comparers.size();
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
                _Hack hack(
                    std::forward<T1>(generator_program), 
                    std::forward<T2>(std_program), 
                    std::forward<T4>(checker_program), 
                    time_limit, 
                    limit_std_runtime ? time_limit : _setting::time_limit_inf,
                    copy_wrong_to_testcase, delete_correct);
                hack.__add_comparer(std::forward<T3>(compare_program), max_try, stop_when_wrong);
                hack.__hack();
                hack.__detail_summary(_msg::_defl);
            }
    } // namespace io
} // namespace generator

#endif // _SGPCET_HACK_H_
