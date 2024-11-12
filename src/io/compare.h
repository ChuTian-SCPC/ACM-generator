#ifndef _SGPCET_COMPARE_H_
#define _SGPCET_COMPARE_H_

#ifndef _SGPCET_IO_REPORTER_H_
#include "io_reporter.h"
#endif // !_SGPCET_IO_REPORTER_H_

namespace generator {
    namespace io {
        std::string checker_name[_enum::MaxChecker] = {
          "lcmp",
          "yesno",
          "rcmp4",
          "rcmp6",
          "rcmp9",
          "wcmp"  
        };

        Path __get_default_checker_file(Checker checker) {
            Path folder_path(__full_path(__path_join(__lib_path().__folder_path(), _setting::_checker_folder)));
        #ifdef ON_WINDOWS
            Path checker_path(__path_join(folder_path, _setting::_sub_checker_folder, __end_with(checker_name[checker], _enum::_EXE)));
        #else
            Path checker_path(__path_join(folder_path, _setting::_sub_checker_folder, checker_name[checker]));
        #endif
            return checker_path;
        }

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


    } // namespace io
} // namespace generator

#endif // !_SGPCET_COMPARE_H_
