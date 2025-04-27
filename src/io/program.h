#ifndef _SGPCET_PROGRAM_H_
#define _SGPCET_PROGRAM_H_

#ifndef _SGPCET_IO_INIT_H_
#include "io_init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {       
        void __open_input_file(const char* file) {
            if (std::cin.eof())  std::cin.clear();
            if (freopen(file, "r", stdin) == NULL) {
                _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_output_file(const char* file) {
            fflush(stdout);
            if (freopen(file, "w", stdout) == NULL) {
                _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open file %s.", file));
            }
        }
        
        void __open_error_file(const char* file) {
            fflush(stderr);
            if (freopen(file, "w", stderr) == NULL) {
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

        void __terminate_process(void* process) {
        #ifdef ON_WINDOWS
            TerminateProcess(reinterpret_cast<HANDLE>(process), 0);           
        #else
            pid_t pid = static_cast<pid_t>(reinterpret_cast<long long>(process));
            kill(pid, SIGTERM);
        #endif
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

        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, int>::type
        __run_child_program(T program, Path input, Path output, Path error, _enum::_FuncProgramType type) {
            if (!input.__empty()) __open_input_file(input);
            if (!output.__empty()) __open_output_file(output);
            if (!error.__empty()) __open_error_file(error);
            __set_args(program.args(), type);
            program.func()();
            return EXIT_SUCCESS;
        }
        
        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, int>::type
        __run_child_program(T program, Path input, Path output, Path error, _enum::_FuncProgramType) {
            if (!input.__empty() && program.enable_default_args()) program.add_args("<", input);
            if (!output.__empty() && program.enable_default_args()) program.add_args(">", output);
            if (!error.__empty() && program.enable_default_args()) program.add_args("2>", error);
            int return_code = program.run();
            if (program.enable_default_args()) program.clear_args();
            return return_code;
        } 

        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __close_files(Path input, Path output, Path error) {
            if (!input.__empty()) __close_input_file_to_console();
            if (!output.__empty()) __close_output_file_to_console();
            if (!error.__empty()) __close_error_file_to_console();
        }   

        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __close_files(Path, Path, Path) {
            return;
        }

        struct ReturnState {
            int exit_code;
            int time;
        };

        #ifdef ON_WINDOWS

        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsCommandPathConstructible<T1>::value &&
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_child_process_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _enum::_FuncProgramType func_type) {
            __set_default_args();
            auto start_time = std::chrono::steady_clock::now();
            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(sa);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;       
            
            HANDLE hInFile = INVALID_HANDLE_VALUE;
            HANDLE hOutFile = INVALID_HANDLE_VALUE;
            HANDLE hErrorFile = INVALID_HANDLE_VALUE;

            Path input_file_path(input_file);
            Path output_file_path(output_file);
            Path error_file_path(error_file);

            if (!input_file_path.__empty()) {
                if (!input_file_path.__file_exist())  return {_setting::_error_return, -1};
                hInFile = CreateFileA(Path(input_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    &sa,
                    OPEN_EXISTING ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );
            }
            
            if (!output_file_path.__empty()) {
                hOutFile = CreateFileA(Path(output_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_WRITE | FILE_SHARE_READ,
                    &sa,
                    CREATE_ALWAYS ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );                
            }

            if (!error_file_path.__empty()) {
                hErrorFile = CreateFileA(Path(error_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_WRITE | FILE_SHARE_READ,
                    &sa,
                    CREATE_ALWAYS ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );                
            }

            PROCESS_INFORMATION pi; 
            STARTUPINFOA si;
            BOOL ret = FALSE; 
            DWORD flags = CREATE_NO_WINDOW;

            ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&si, sizeof(STARTUPINFOA));
            si.cb = sizeof(STARTUPINFOA); 
            si.dwFlags |= STARTF_USESTDHANDLES;
            si.hStdInput = hInFile != INVALID_HANDLE_VALUE ? hInFile : NULL;
            si.hStdError = hErrorFile != INVALID_HANDLE_VALUE ? hErrorFile : NULL;
            si.hStdOutput = hOutFile != INVALID_HANDLE_VALUE ? hOutFile : NULL;
            ret = CreateProcessA(NULL, const_cast<char *>(program.command().c_str()), NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
            if (ret) 
            {
                if (__is_time_limit_inf(time_limit))  WaitForSingleObject(pi.hProcess, INFINITE);
                else if (WaitForSingleObject(pi.hProcess, time_limit) == WAIT_TIMEOUT) __terminate_process(pi.hProcess);
                auto end_time = std::chrono::steady_clock::now();
                DWORD exitCode;
                GetExitCodeProcess(pi.hProcess, &exitCode);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                return {(int)exitCode, (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()};
            }
            else {
                if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                return {_setting::_error_return, -1};
            }   
        }
        
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsCommandFuncConstructible<T1>::value &&   
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_child_process_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _enum::_FuncProgramType func_type) {
            __set_default_args();
            if (__is_time_limit_inf(time_limit)) {
                auto start_time = std::chrono::steady_clock::now();
                Path input_file_path(input_file);
                Path output_file_path(output_file);
                Path error_file_path(error_file);
                int exit_code = __run_child_program(program, input_file_path, output_file_path, error_file_path, func_type);
                __close_files<T1>(input_file_path, output_file_path, error_file_path);
                auto end_time = std::chrono::steady_clock::now();
                return {exit_code, (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()};
            } else {
                _msg::__error_msg(_msg::_defl, "Unsupport running time limit function in Windows");
                return {_setting::_error_return, -1};
            }
        }
        
        #else
        
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgram<T1>::value &&   
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_child_process_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _enum::_FuncProgramType func_type) {
            auto start_time = std::chrono::steady_clock::now();
            pid_t pid = fork();
            Path input_file_path(input_file);
            Path output_file_path(output_file);
            Path error_file_path(error_file);            
            if (pid == 0) {
                __set_default_args();
                int exit_code = __run_child_program(program, input_file_path, output_file_path, error_file_path, func_type);
                __close_files<T1>(input_file_path, output_file_path, error_file_path);
                exit(exit_code);           
            } 
            else if (pid > 0) {

                __set_default_args();
                auto limit = std::chrono::milliseconds(time_limit);

                int status;
                
                if (__is_time_limit_inf(time_limit)) {
                    waitpid(pid, &status, 0);
                }
                else {
                    auto result = waitpid(pid, &status, WNOHANG);
                    while (result == 0 && std::chrono::steady_clock::now() - start_time < limit) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        result = waitpid(pid, &status, WNOHANG);
                    }     
                    if (result == 0) {
                        __terminate_process(reinterpret_cast<void*>(pid));
                    }   
                    result = waitpid(pid, &status, WNOHANG);              
                }
                
                auto end_time = std::chrono::steady_clock::now();              
                int exit_status = WEXITSTATUS(status);
                if (WIFEXITED(status) && exit_status == -1) {     
                    _msg::__warn_msg(_msg::_defl, "Fail to run program or something error.");
                    return {_setting::_error_return, -1};
                } else {
                    return {exit_status, (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()};   
                }    
                     
            } 
            else {
                _msg::__warn_msg(_msg::_defl, "Fail to fork.");
                return {_setting::_error_return, -1};
            }            
        }    
        #endif

        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __check_program_valid(T program) {
            program.path().full();
            if (!program.path().__file_exist()) {
                _msg::__error_msg(_msg::_defl, tools::string_format("File %s not exist.", program.path().cname()));
            }
        }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __check_program_valid(T) {
            return;
        }

        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, void>::type
        __check_program_valid(T program) {
            Path program_path(program);
            program_path.full();
            if (!program_path.__file_exist()) {
                _msg::__error_msg(_msg::_defl, tools::string_format("File %s not exist.", program_path.cname()));
            }
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, void>::type
        __check_program_valid(T) {
            return;
        }

        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgram<T1>::value &&
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _enum::_FuncProgramType func_type) {
            __check_program_valid(program);
            return __run_child_process_program(program, input_file, output_file, error_file, time_limit, func_type);
        }

        bool __is_error(int return_code) {
            return return_code == _setting::_error_return;
        }
        
        bool __is_success(int return_code) {
            return return_code == EXIT_SUCCESS;
        }

    } // namespace io
} // namespace generator

#endif // !_SGPCET_PROGRAM_H_
