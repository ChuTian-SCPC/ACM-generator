#ifndef _SGPCET_IO_INIT_H_
#define _SGPCET_IO_INIT_H_

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
        struct IsCommandPath {
            static constexpr bool value = std::is_same<T, CommandPath>::value;
        };
        
        template<typename T>
        struct IsCommandFunc {
            static constexpr bool value = std::is_same<T, CommandFunc>::value;
        }; 
        
        template<typename T>
        struct IsProgram {
            static constexpr bool value = std::is_base_of<_Program, T>::value;
        };
        
        template<typename T>
        struct IsProgramConstructible {
            static constexpr bool value = 
                IsCommandPathConstructible<T>::value || 
                IsCommandFuncConstructible<T>::value || 
                IsPathConstructible<T>::value || 
                IsFunctionConvertible<T>::value;
        };

        template<bool...> struct BoolPack;
        template<bool... Args> 
        using ArgsAllTrue = std::is_same<BoolPack<Args..., true>, BoolPack<true, Args...>>;

        void __ensure_file_folder(Path file) {
            __create_directories(file.__folder_path());
        }                           

        Path __testcases_folder() {
            return __path_join(__current_path(), _setting::testcase_folder);
        }

        Path __generate_log_folder(const Path& folder) {
            return __path_join(folder, _setting::generate_log_folder);
        }

        Path __generate_log_folder() {
            return __generate_log_folder(__current_path());
        }

        std::string _file_end[_enum::_MAX_END] = {
            _setting::input_suffix,
            _setting::output_suffix,
            ".ans",
            ".log",
            ".logc",
            ".exe",
            ".val",
            ".check",
            ".glog"
        };

        Path __test_folder() {
            return __path_join(__current_path(), _setting::test_folder);
        }

        Path __test_validator_folder() {
            return __path_join(__test_folder(), _setting::test_validator_sub_folder);
        }

        Path __test_checker_folder() {
            return __path_join(__test_folder(), _setting::test_checker_sub_folder);
        }

        void __check_input_output_suffix() {
            if (_setting::input_suffix != _file_end[_enum::_IN]) _file_end[_enum::_IN] = _setting::input_suffix;
            if (_setting::output_suffix != _file_end[_enum::_OUT]) _file_end[_enum::_OUT] = _setting::output_suffix;
        }

        std::string __end_with(int x, _enum::_End end) {
            __check_input_output_suffix();
            return std::to_string(x) + _file_end[end];
        }
        
        std::string __end_with(const char* text, _enum::_End end) {
            __check_input_output_suffix();
            return std::string(text) + _file_end[end];
        }
        
        std::string __end_with(std::string text, _enum::_End end) {
            __check_input_output_suffix();
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
            std::string inputs_format = "*" + _setting::input_suffix;
            HANDLE hFind = FindFirstFile(folder_path.join(inputs_format).cname(), &findFileData);

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
                    size_t suffix_len = _setting::input_suffix.size();
                    if (file_name.size() >= suffix_len && file_name.substr(file_name.size() - suffix_len) == _setting::input_suffix) {
                        int num = std::stoi(file_name.substr(0, file_name.size() - suffix_len));
                        inputs.emplace_back(num);
                    }
                }
                closedir(dir);
            }
        #endif

            return inputs;
        } 

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, _Program*>::type
        __program(T program) {
            return program.__clone();
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, _Program*>::type
        __program(T* program) {
            return program->__clone();
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, _Program*>::type
        __program(const T& program) {
            return new CommandFunc(program);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, _Program*>::type
        __program(const T& program) {
            return new CommandPath(program);
        }

        template<typename T>
        _Program* __generator_program(T&& program, int x, bool hack = false) {
            _Program* p = __program(std::forward<T>(program));
            if (p->enable_default_args()) {
                if (hack) p->add_args(_setting::default_hack_stable_seed + std::to_string(x));
                else p->add_args(_setting::default_stable_seed + std::to_string(x));
            }
            return p;
        }

        template<typename T>
        _Program* __result_program(T&& program) {
            return __program(std::forward<T>(program));
        }

        template<typename T>
        struct IsDefaultChecker {
            static constexpr bool value = std::is_same<T, _enum::Checker>::value;
        };

        template<typename T>
        struct IsCheckerConstructible {
            static constexpr bool value = IsDefaultChecker<T>::value || IsProgramConstructible<T>::value;
        };

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
        typename std::enable_if<!IsDefaultChecker<T>::value, _Program*>::type
        __checker_program(T&& program) {
            return __program(std::forward<T>(program));
        }

        template<typename T>
        typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
        __checker_program(T&& program) {
            _Program* func = new CommandPath(__get_default_checker_file(program));
            return func;
        }
        
        Path __compare_folder() {
            return __path_join(__current_path(), _setting::compare_folder);
        }

        Path __hack_folder() {
            return __path_join(__current_path(), _setting::hack_folder);
        }

        template<typename T>
        _Program* __validator_program(T&& program) {
            return __program(std::forward<T>(program));
        }
        
        Path __validate_folder(std::string case_name) {
            return __path_join(__current_path(), _setting::validate_folder, case_name);
        }

        int _judge_result_priority[__state_index(_enum::_JudgeState::_JUDGE_STATE_MAX)] = {
            0, // UNKNOWN
            1, // AC
            2, // WA
            6, // RE
            5, // TLE
            3, // TLE AC
            4  // TLE WA
        };

    } // namespace generator
} // namespace io

#endif // !_SGPCET_IO_INIT_H_
