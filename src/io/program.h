#ifndef _SGPCET_PROGRAM_H_
#define _SGPCET_PROGRAM_H_

#ifndef _SGPCET_COMMON_H_
#include "common/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_ENUM_H_
#include "common/enum.h"
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
        std::string _file_end[_enum::_MAX_END] = {
            ".in",
            ".out",
            ".ans",
            ".log",
            ".logc",
            ".exe",
            ".val"
        };

        std::string __end_with(int x, _End end) {
            return std::to_string(x) + _file_end[end];
        }
        
        std::string __end_with(const char* text, _End end) {
            return std::string(text) + _file_end[end];
        }
        
        std::string __end_with(std::string text, _End end) {
            return text + _file_end[end];
        }
        
        void __open_input_file(const char* file) {
            if (std::cin.eof())  std::cin.clear();
            if (freopen(file, "r", stdin) == NULL) {
                _msg::__error_msg(_msg::_default_log, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_output_file(const char* file) {
            fflush(stdout);
            if (freopen(file, "w", stdout) == NULL) {
                _msg::__error_msg(_msg::_default_log, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_error_file(const char* file) {
            fflush(stderr);
            if (freopen(file, "w", stderr) == NULL) {
                _msg::__error_msg(_msg::_default_log, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_input_file(Path path) {
            __open_input_file(path.cname());
        }
        
        void __open_output_file(Path path) {
            __open_output_file(path.cname());
        }
        
        void __open_error_file(Path path) {
            __open_error_file(path.cname());
        }
        
        void __close_input_file_to_console() {
        #ifdef ON_WINDOWS
            __open_input_file("CON");
        #else
            __open_input_file("/dev/tty");
        #endif
        }
        
        void __close_output_file_to_console() {
        #ifdef ON_WINDOWS
            __open_output_file("CON");
        #else
            __open_output_file("/dev/tty");
        #endif
        }
        
        void __close_error_file_to_console() {
        #ifdef ON_WINDOWS
            __open_error_file("CON");
        #else
            __open_error_file("/dev/tty");
        #endif 
        }

        Path __testcases_folder() {
            return __path_join(__current_path(), _setting::_testcase_folder);
        }
        
        void __ensure_file_folder(Path file) {
            __create_directories(file.__folder_path());
        }
        
        Path __input_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _enum::_IN));
        }
        
        Path __output_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _enum::_OUT));
        }
        
        bool __input_file_exists(int x) {
            return __input_file_path(x).__file_exists();
        }
        
        bool __output_file_exists(int x) {
            return __output_file_path(x).__file_exists();
        }

        std::vector<int> __get_all_inputs() {
            std::vector<int> inputs;
            Path folder_path = __testcases_folder();
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

        void __terminate_process(void* process) {
        #ifdef ON_WINDOWS
            TerminateProcess(reinterpret_cast<HANDLE>(process), 0);           
        #else
            pid_t pid = static_cast<pid_t>(reinterpret_cast<long long>(process));
            kill(pid, SIGTERM);
        #endif
        }

        bool __is_time_limit_inf(int time_limit) {
            return time_limit == _setting::time_limit_inf;
        }

        int __time_limit_extend(int time_Limit) {
            if (__is_time_limit_inf(time_Limit)) return time_Limit;
            return time_Limit * _setting::time_limit_over_ratio;
        }

        char** __split_string_to_char_array(const char* input) {
            char** char_array = nullptr;
            char* cinput = const_cast<char*>(input);
            char* token = strtok(cinput, " ");
            int count = 0;

            while (token != nullptr) {
                char_array = (char**)realloc(char_array, (count + 1) * sizeof(char*));
                char_array[count] = strdup(token);
                ++count;
                token = strtok(nullptr, " ");
            }

            char_array = (char**)realloc(char_array, (count + 1) * sizeof(char*));
            char_array[count] = nullptr;
            return char_array;
        }

        void __free_char_array(char** char_array) {
            if (char_array != nullptr) {
                for (int i = 0; char_array[i] != nullptr; ++i) {
                    free(char_array[i]); 
                }
                free(char_array); 
            }
        }
        
        int __fake_argc(char** fake_argvs) {
            int fake_argc = 0;
            while (fake_argvs[fake_argc] != nullptr) {
                ++fake_argc;
            }
            return fake_argc;    
        }
        
        char** __fake_argv(std::string stage, std::string args = ""){
            args = stage + " " + args;
            return __split_string_to_char_array(args.c_str());
        }
        
        void init_gen(int argc,char* argv[]) {
            registerGen(argc, argv, 1);
        }

        void __set_generator_args(std::string args) {
            char** fake_argv = __fake_argv(_setting::_first_generator_argv, args);
            int fake_argc = __fake_argc(fake_argv);
            registerGen(fake_argc, fake_argv , 1);
            __free_char_array(fake_argv);
        }

        void init_gen() {
            __set_generator_args("");
        }
        
        void __set_checker_args(std::string args) {
            char** fake_argv = __fake_argv(_setting::_first_checker_argv, args);
            int fake_argc = __fake_argc(fake_argv);
            registerTestlibCmd(fake_argc, fake_argv);
            __free_char_array(fake_argv);
        }
        
        void __set_validator_args(std::string args) {
            if (args.empty()) {
                registerValidation();
            }
            else {
                char** fake_argv = __fake_argv(_setting::_first_validator_argv, args);
                int fake_argc = __fake_argc(fake_argv);
                registerValidation(fake_argc, fake_argv);     
                __free_char_array(fake_argv);           
            }
        }
        
        void __set_default_args() {
            __set_generator_args("");
        }
        
        void __set_args(std::string args, _enum::_FuncProgramType type) {
            switch(type) {
            case _enum::_GENERATOR:
                __set_generator_args(args);
                break;
            case _enum::_CHECKER:
                __set_checker_args(args);
                break;
            case _enum::_VALIDATOR:
                __set_validator_args(args);
                break;
            default:
                __set_default_args();
                break;
            }
        }

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
    } // namespace io
} // namespace generator

#endif // !_SGPCET_PROGRAM_H_