#ifndef _SGPCET_IO_INIT_H_
#define _SGPCET_IO_INIT_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_ENUM_H_
#include "basic/enum.h"
#endif // !_SGPCET_ENUM_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_COMMAND_PATH_H_
#include "command_path.h"
#endif // !_SGPCET_COMMAND_PATH_H_
#ifndef _SGPCET_COMMAND_FUNC_H_
#include "command_func.h"
#endif // !_SGPCET_COMMAND_FUNC_H_

namespace generator {
    namespace io {

        template<typename T>
        struct IsCommandPathConstructible {
            static constexpr bool value = std::is_constructible<T, CommandPath>::value;
        };
        
        template<typename T>
        struct IsCommandFuncConstructible {
            static constexpr bool value = std::is_constructible<T, CommandFunc>::value;
        };
        
        template<typename T>
        struct IsProgram {
            static constexpr bool value = IsCommandPathConstructible<T>::value || IsCommandFuncConstructible<T>::value;
        };
        
        template<typename T>
        struct IsProgramConstructible {
            static constexpr bool value = IsProgram<T>::value || IsPathConstructible<T>::value || IsFunctionConvertible<T>::value;
        };
        
        template<typename T>
        struct _ProgramType {
            using type = typename std::conditional<
                IsCommandPathConstructible<T>::value || IsPathConstructible<T>::value,   
                CommandPath,
                CommandFunc >::type;
        };

        template<typename T>
        using _ProgramTypeT = typename _ProgramType<T>::type;

        void __ensure_file_folder(Path file) {
            __create_directories(file.__folder_path());
        }                           

        Path __testcases_folder() {
            return __path_join(__current_path(), _setting::testcase_folder);
        }

        std::string _file_end[_enum::_MAX_END] = {
            ".in",
            ".out",
            ".ans",
            ".log",
            ".logc",
            ".exe",
            ".val"
        };

        std::string __end_with(int x, _enum::_End end) {
            return std::to_string(x) + _file_end[end];
        }
        
        std::string __end_with(const char* text, _enum::_End end) {
            return std::string(text) + _file_end[end];
        }
        
        std::string __end_with(std::string text, _enum::_End end) {
            return text + _file_end[end];
        }

        Path __input_file_path(Path folder, int x) {
            return __path_join(folder, __end_with(x, _enum::_IN));
        }
        
        Path __output_file_path(Path folder, int x) {
            return __path_join(folder, __end_with(x, _enum::_OUT));
        }

        Path __testcase_input_file_path(int x) {
            return __input_file_path(__testcases_folder(), x);
        }
        
        Path __testcase_output_file_path(int x) {
            return __output_file_path(__testcases_folder(), x);
        }

        bool __input_file_exists(Path folder, int x) {
            return __input_file_path(folder, x).__file_exist();
        }
        
        bool __output_file_exists(Path folder, int x) {
            return __output_file_path(folder, x).__file_exist();
        }

        bool __testcase_input_file_exists(int x) {
            return __testcase_input_file_path(x).__file_exist();
        }
        
        bool __testcase_output_file_exists(int x) {
            return __testcase_output_file_path(x).__file_exist();
        }

        std::vector<int> __get_all_inputs(std::string case_name = _setting::testcase_folder) {
            std::vector<int> inputs;
            Path folder_path = __path_join(__current_path(), case_name);
        #ifdef ON_WINDOWS
            WIN32_FIND_DATA findFileData;
            HANDLE hFind = FindFirstFile(folder_path.join("*.in").cname(), &findFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    Path file_path(__path_join(folder_path,findFileData.cFileName));
                    int num = std::stoi(file_path.__file_name());  
                    inputs.emplace_back(num);  
                } while (FindNextFile(hFind, &findFileData) != 0);

                FindClose(hFind);
            }
        #else
            DIR* dir = opendir(folder_path.cname());
            if (dir != nullptr) {
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::string file_name = entry->d_name;
                    if (file_name.size() >= 3 && file_name.substr(file_name.size() - 3) == ".in") {
                        int num = std::stoi(file_name.substr(0, file_name.size() - 3));
                        inputs.emplace_back(num);
                    }
                }
                closedir(dir);
            }
        #endif

            return inputs;
        } 

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __generator_program(T program, int x, bool hack = false) {
            if (program.enable_default_args()) {
                if (hack) program.add_args(_setting::default_hack_stable_seed + std::to_string(x));
                else program.add_args(_setting::default_stable_seed + std::to_string(x));
            }
            return program;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __generator_program(T program, int x, bool hack = false) {
            std::string args = "";
            if (_setting::default_seed) {
                if (hack) args = _setting::default_hack_stable_seed + std::to_string(x);
                else args =  _setting::default_stable_seed + std::to_string(x);
            }
            return CommandFunc(program, args);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __generator_program(T program, int x, bool hack = false) {
            std::string args = "";
            if (_setting::default_seed) {
                if (hack) args = _setting::default_hack_stable_seed + std::to_string(x);
                else args =  _setting::default_stable_seed + std::to_string(x);
            }
            return CommandPath(program, args);
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __result_program(T program) {
            return program;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __result_program(T program) {
            return CommandFunc(program);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __result_program(T program) {
            return CommandPath(program);
        } 

        template<typename T>
        struct IsDefaultChecker {
            static constexpr bool value = std::is_same<T, _enum::Checker>::value;
        };
        
        template<typename T>
        struct IsCheckerConstructible {
            static constexpr bool value = IsProgramConstructible<T>::value || IsDefaultChecker<T>::value;
        };
        
        template<typename T>
        struct _CheckerType {
            using type = typename std::conditional<
                IsCommandPathConstructible<T>::value || IsPathConstructible<T>::value ||IsDefaultChecker<T>::value,   
                CommandPath,
                CommandFunc >::type;
        };

        template<typename T>
        using _CheckerTypeT = typename _CheckerType<T>::type;

        std::string checker_name[_enum::MaxChecker] = {
          "lcmp",
          "yesno",
          "rcmp4",
          "rcmp6",
          "rcmp9",
          "wcmp"  
        };

        Path __get_default_checker_file(_enum::Checker checker) {
            Path folder_path(__full_path(__path_join(Path(__lib_path()).__folder_path(), _setting::_checker_folder)));
        #ifdef ON_WINDOWS
            Path checker_path(__path_join(folder_path, _setting::_sub_checker_folder, __end_with(checker_name[checker], _enum::_EXE)));
        #else
            Path checker_path(__path_join(folder_path, _setting::_sub_checker_folder, checker_name[checker]));
        #endif
            return checker_path;
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, _ProgramTypeT<T>>::type
        __checker_porgram(T program) {
            return __result_program(program);
        }

        template<typename T>
        typename std::enable_if<IsDefaultChecker<T>::value, CommandPath>::type
        __checker_porgram(T program) {
            CommandPath func(__get_default_checker_file(program));
            return func;
        }
        
        Path __compare_folder() {
            return __path_join(__current_path(), _setting::compare_folder);
        }

        bool __is_time_limit_inf(int time_limit) {
            return time_limit == _setting::time_limit_inf;
        }

        bool __time_limit_exceed(int time, int time_limit) {
            return !__is_time_limit_inf(time_limit) && time > time_limit;
        }

        int __time_limit_extend(int time_Limit) {
            if (__is_time_limit_inf(time_Limit)) return time_Limit;
            return time_Limit * _setting::time_limit_over_ratio;
        }

        Path __hack_folder() {
            return __path_join(__current_path(), _setting::hack_folder);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, _ProgramTypeT<T>>::type
        __validator_program(T program) {
            return __result_program(program);
        }
        
        Path __validate_folder(std::string case_name) {
            return __path_join(__current_path(), _setting::validate_folder, case_name);
        }

    } // namespace generator
} // namespace io

#endif // !_SGPCET_IO_INIT_H_
