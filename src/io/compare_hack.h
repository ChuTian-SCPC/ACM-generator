#ifndef _SGPCET_COMPARE_HACK_H_
#define _SGPCET_COMPARE_HACK_H_

#ifndef _SGPCET_IO_REPORTER_H_
#include "io_reporter.h"
#endif // !_SGPCET_IO_REPORTER_H_

namespace generator {
    namespace io {
        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, std::string>::type
        __compare_program_name(T program) {
            return program.path().__file_name();
        }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, std::string>::type
        __compare_program_name(T) {
            std::string name = "function" + std::to_string(_setting::_function_count);
            return name;
        }

        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __increase_function_count() { return; }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __increase_function_count() { _setting::_function_count++; }
        
        template<typename T>
        Path __compare_program_folder(T program) {
            return __path_join(__compare_folder(), __compare_program_name(program));
        }
        
        bool __enable_judge_ans(int runtime, int time_limit, _enum::_JudgeState& result) {
            if (!__time_limit_exceed(runtime, time_limit)) return true;
            result = _enum::_JudgeState::_TLE;
            return runtime <= time_limit * _setting::time_limit_check_ratio;
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, void>::type
        __check_result(Path& input_file, Path& std_output_file, Path& ans_file, Path& testlib_out_file,
            T checker, _enum::_JudgeState& result, std::string& testlib_result) {
            checker.add_args(input_file, ans_file, std_output_file);
            __run_program(checker, _setting::_default_path, _setting::_default_path, 
                testlib_out_file, _setting::time_limit_inf, _enum::_CHECKER);       
            std::ifstream check_stream(testlib_out_file.path());
            std::string line;
            while(check_stream >> line){
                testlib_result += line;
                testlib_result += " ";
            }
            check_stream.close();            
            if (testlib_result.substr(0, 2) == "ok") result |= _enum::_JudgeState::_AC;
            else result |= _enum::_JudgeState::_WA;
        } 

        template<typename T, typename F>
        typename std::enable_if<IsProgram<T>::value && IsProgramConstructible<F>::value, void>::type
        __check_once(int id, F program, int time_limit, T checker, Path& ans_file, Path& testlib_out_file, 
            int& runtime, _enum::_JudgeState& result, std::string& testlib_result) {
            Path input_file = __input_file_path(id);
            Path output_file = __output_file_path(id);
            ReturnState state = __run_program(program, input_file, ans_file, 
                _setting::_default_path, __time_limit_extend(time_limit), _enum::_RESULT);
            if (!__is_success(state.exit_code)) {
                result = _enum::_JudgeState::_ERROR;
                return;
            }
            runtime = state.time;
            if(__enable_judge_ans(runtime, time_limit, result)) {
                __check_result(
                    input_file, output_file, ans_file, testlib_out_file,
                    checker, result, testlib_result);
            }   
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, void>::type
        __compare_impl(std::map<int, int>& , int , T ) {
            return;
        } 

        template<typename T, typename F, typename... Args>
        typename std::enable_if<IsProgram<T>::value && IsProgramConstructible<F>::value, void>::type
        __compare_impl(std::map<int, int>& case_indices, int time_limit, T checker, F first, Args... args) {
            _ProgramTypeT<F> program = __result_program(first);
            __check_program_valid(program);
            __increase_function_count<_ProgramTypeT<F>>();
            Path ans_folder = __compare_program_folder(program);
            std::string program_name = __compare_program_name(program);
            __create_directories(ans_folder);
            Path testlib_out_file = __path_join(ans_folder, __end_with("__checker", _enum::_LOGC));
            int case_count = case_indices.size();
            std::vector<int> runtimes(case_count, -1);
            std::vector<_enum::_JudgeState> results(case_count, _enum::_JudgeState::_UNKNOWN);
            std::vector<std::string> testlib_results(case_count);
            std::vector<int> results_count(_enum::__state_index(_enum::_JudgeState::_JUDGE_STATE_MAX), 0);
            _msg::__info_msg(_msg::_defl, "Test results for program ", _msg::_ColorMsg(program_name, _enum::Color::Green), " :");
            Path log_path = __path_join(__compare_folder(), __end_with(program_name, _enum::_LOG));
            _msg::OutStream log(log_path); 
            for (auto cas : case_indices) {
                int real_index = cas.first;
                int vec_index = cas.second;
                Path ans_file = __path_join(ans_folder, __end_with(real_index, _enum::_ANS));
                __check_once(
                    real_index, program, time_limit, checker, ans_file, testlib_out_file,
                    runtimes[vec_index], results[vec_index], testlib_results[vec_index]);
                results_count[_enum::__state_index(results[vec_index])]++;
                __judge_msg(_msg::_defl, results[vec_index], real_index, runtimes[vec_index], testlib_results[vec_index]);
                __judge_msg(log, results[vec_index], real_index, runtimes[vec_index], testlib_results[vec_index]);
            }
            testlib_out_file.__delete_file();
            __report_compare_logs(case_count, log, results_count);
            __compare_impl(case_indices, time_limit, checker, args...);
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int start, int end, int time_limit, T checker, Args... args) {
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Compare", _enum::Color::Green));
            std::map<int, int> case_indices;
            int count = 0;
            _CheckerTypeT<T> checker_program = __checker_porgram(checker);
            for (int i = start; i <= end; i++) {
                if (__input_file_exists(i) && __output_file_exists(i)) {
                    case_indices[i] = count;
                    count ++;
                }
            }
            __compare_impl(case_indices, time_limit, checker_program, args...);
        }
        
        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int time_limit, T checker, Args... args) {
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Compare", _enum::Color::Green));
            std::map<int, int> case_indices;
            int count = 0;
            _CheckerTypeT<T> checker_program = __checker_porgram(checker);
            for (int i : __get_all_inputs()) {
                if (__output_file_exists(i)) {
                    case_indices[i] = count;
                    count++;
                }
            }
            __compare_impl(case_indices, time_limit, checker_program, args...);
        }

        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgramConstructible<T1>::value &&
            IsProgramConstructible<T2>::value &&
            IsProgramConstructible<T3>::value &&
            IsCheckerConstructible<T4>::value
        >::type
        hack(T1 generator_program, T2 std_program, T3 compare_program, T4 checker_program, int time_limit, bool limit_std_runtime = false,
            int max_try = 100, bool stop_when_wrong = true, bool copy_wrong_to_testcase = true, bool delete_correct = true) {
            if (max_try <= 0) _msg::__fail_msg(_msg::_defl, "At least try once.");
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Hack", _enum::Color::Green));
            _ProgramTypeT<T2> standard = __result_program(std_program);
            _ProgramTypeT<T3> other = __result_program(compare_program);
            _CheckerTypeT<T4> check = __checker_porgram(checker_program);
            __check_program_valid(standard);
            __check_program_valid(other);
            __check_program_valid(check);
            __create_directories(__hack_folder());
            bool all_correct = true;
            for (int i = 1; i <= max_try; i++) {
                Path input = __path_join(__hack_folder(), __end_with(i, _enum::_IN));
                Path output = __path_join(__hack_folder(), __end_with(i, _enum::_OUT));
                Path user = __path_join(__hack_folder(), __end_with(i, _enum::_ANS));
                
                ReturnState gen_state =__run_program(__generator_program(generator_program, i, true), 
                    _setting::_default_path, input, _setting::_default_path, 
                    _setting::time_limit_inf, _enum::_GENERATOR);
                if (!__is_success(gen_state.exit_code)) _msg::__fail_msg(_msg::_defl, "generator meet error.");
                
                int std_time_limit = limit_std_runtime ? time_limit : _setting::time_limit_inf;
                ReturnState standard_state = __run_program(standard, input, output, _setting::_default_path,
                    std_time_limit , _enum::_RESULT);
                if (!__is_success(standard_state.exit_code)) _msg::__fail_msg(_msg::_defl, "std meet error.");
                if (__time_limit_exceed(standard_state.time, std_time_limit)) 
                    _msg::__fail_msg(_msg::_defl, tools::string_format(
                        "std using time(%dms) is greater than the time limit(%dms).", 
                        standard_state.time, std_time_limit));
                
                bool uncorrect = false;
                ReturnState compare_state = __run_program(other, input, user, _setting::_default_path, 
                    time_limit, _enum::_RESULT);
                if (!__is_success(compare_state.exit_code)) _msg::__fail_msg(_msg::_defl, "compare meet error.");              
                if (__time_limit_exceed(compare_state.time, time_limit)) {
                    uncorrect = true;
                    __judge_msg(_msg::_defl, _enum::_JudgeState::_TLE, i, compare_state.time, "");
                }         
                else {
                    Path error_log = __path_join(__hack_folder(), __end_with("__hack", _enum::_LOGC));
                    _enum::_JudgeState state = _enum::_JudgeState::_UNKNOWN;
                    std::string testlib_log;
                    __check_result(input, output, user, error_log, check, state, testlib_log);
                    if (state != _enum::_JudgeState::_AC) {
                        uncorrect = true;
                        __judge_msg(_msg::_defl, _enum::_JudgeState::_WA, i, compare_state.time, testlib_log);
                    }
                    error_log.__delete_file();
                }
                if (uncorrect) {
                    all_correct = false;
                    if (copy_wrong_to_testcase) {
                        std::vector<int> next_input = __find_not_exist_inputs(1);
                        if (next_input.size() == 1) {
                            Path testcase_input = __input_file_path(next_input[0]);
                            Path testcase_output = __output_file_path(next_input[0]);
                            __copy_file(input, testcase_input);
                            __copy_file(output, testcase_output);
                            _msg::__info_msg(_msg::_defl, "Standard input and output is moved to testcase folder:");
                            _msg::__info_msg(_msg::_defl, tools::string_format("  input : %s", testcase_input.cname()));
                            _msg::__info_msg(_msg::_defl, tools::string_format("  output : %s", testcase_output.cname()));                          
                        }
                    }
                    if (stop_when_wrong) break;
                }
                else {
                    __judge_msg(_msg::_defl, _enum::_JudgeState::_AC, i, compare_state.time, "");
                    if (delete_correct) {
                        input.__delete_file();
                        output.__delete_file();
                        user.__delete_file();
                    }
                }
            }
            if (all_correct) {
                __info_msg(_msg::_defl, tools::string_format("Test %d cases(s), ", max_try), 
                    _msg::_ColorMsg("All Passed", _enum::Color::Green));
            }
            _msg::__endl(_msg::_defl);          
        }

    } // namespace io
} // namespace generator

#endif // !_SGPCET_COMPARE_HACK_H_
