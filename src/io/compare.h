#ifndef _SGPCET_COMPARE_H_
#define _SGPCET_COMPARE_H_

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
    } // namespace io
} // namespace generator

#endif // !_SGPCET_COMPARE_H_
