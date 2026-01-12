#ifndef _SGPCET_FILE_H_
#define _SGPCET_FILE_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_ENUM_H_
#include "basic/enum.h"
#endif // !_SGPCET_ENUM_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_PATH_H_
#include "path.h"
#endif // !_SGPCET_PATH_H_

namespace generator {
    namespace io {
        static int original_stdin = -1;
        static int original_stdout = -1;
        static int original_stderr = -1;
        
        void __open_input_file(const char* file) {
            if (original_stdin == -1) {
                original_stdin = dup(fileno(stdin));  // 保存原始的 stdin
            }
            if (std::cin.eof()) std::cin.clear();
            if (freopen(file, "r", stdin) == nullptr) {
                _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_output_file(const char* file) {
            if (original_stdout == -1) {
                original_stdout = dup(fileno(stdout));  // 保存原始的 stdout
            }
            fflush(stdout);
            if (freopen(file, "w", stdout) == nullptr) {
                _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_error_file(const char* file) {
            if (original_stderr == -1) {
                original_stderr = dup(fileno(stderr));  // 保存原始的 stderr
            }
            fflush(stderr);
            if (freopen(file, "w", stderr) == nullptr) {
                _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open file %s.", file));
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
            if (original_stdin != -1) {
                dup2(original_stdin, fileno(stdin));  // 恢复 stdin
                close(original_stdin);
                original_stdin = -1;
            }
        }
        
        void __close_output_file_to_console() {
            fflush(stdout);
            if (original_stdout != -1) {
                dup2(original_stdout, fileno(stdout));  // 恢复 stdout
                close(original_stdout);
                original_stdout = -1;
            }
        }
        
        void __close_error_file_to_console() {
            fflush(stderr);
            if (original_stderr != -1) {
                dup2(original_stderr, fileno(stderr));  // 恢复 stderr
                close(original_stderr);
                original_stderr = -1;
            }
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

        void __set_generator_args(std::string args = "") {
            char** fake_argv = __fake_argv(_setting::_first_generator_argv, args);
            int fake_argc = __fake_argc(fake_argv);
            registerGen(fake_argc, fake_argv , 1);
            __free_char_array(fake_argv);
        }

        void init_gen() {
            __set_generator_args();
        }
        
        void init_checker(int argc,char* argv[]) {
            registerTestlibCmd(argc, argv);
        }

        void __set_checker_args(std::string args = "") {
            char** fake_argv = __fake_argv(_setting::_first_checker_argv, args);
            int fake_argc = __fake_argc(fake_argv);
            registerTestlibCmd(fake_argc, fake_argv);
            __free_char_array(fake_argv);
        }

        void init_checker() {
            __set_checker_args();
        }
        
        void init_validator(int argc,char* argv[]) {
            registerValidation(argc, argv);
        }

        void init_validator() {
            registerValidation();
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
            if (type == _enum::_GENERATOR) __set_generator_args(args);
            else if (type == _enum::_CHECKER) __set_checker_args(args);
            else if (type == _enum::_VALIDATOR) __set_validator_args(args);
            else __set_default_args();
        }
    }
}

#endif
