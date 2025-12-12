#ifndef _SGPCET_PROGRAM_H_
#define _SGPCET_PROGRAM_H_

#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_FILE_H_
#include "file.h"
#endif // !_SGPCET_FILE_H_

namespace generator {
    namespace io {       
        struct ReturnState {
            int exit_code;
            int time;
        };

        bool __is_error(int return_code) {
            return return_code == _setting::_error_return;
        }
        
        bool __is_success(int return_code) {
            return return_code == EXIT_SUCCESS;
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

        class _Program {
        protected:
            std::string _args;
            bool _enable_default_args;
            std::string _name;
        public:
            _Program() : _args(""), _enable_default_args(true), _name("") {}
            _Program(std::string args) : _args(args), _enable_default_args(false), _name("") {}
            _Program(const _Program& other) : _args(other._args), _enable_default_args(other._enable_default_args), _name(other._name) {}
            _Program& operator=(const _Program& other) {
                if (this != &other) {
                    _args = other._args;
                    _enable_default_args = other._enable_default_args;
                    _name = other._name;
                }
                return *this;
            }
            _Program(_Program&& other) noexcept : _args(std::move(other._args)), _enable_default_args(other._enable_default_args), _name(std::move(other._name)) {}
            _Program& operator=(_Program&& other) noexcept {
                if (this != &other) {
                    _args = std::move(other._args);
                    _enable_default_args = other._enable_default_args;
                    _name = std::move(other._name);
                }
                return *this;
            }
        protected:
            void __terminate_process(void* process) {
            #ifdef ON_WINDOWS
                TerminateProcess(reinterpret_cast<HANDLE>(process), 0);           
            #else
                pid_t pid = static_cast<pid_t>(reinterpret_cast<long long>(process));
                kill(pid, SIGTERM);
            #endif
            }

            virtual int __run_child_program(Path input, Path output, Path error, _enum::_FuncProgramType type) = 0;

            virtual void __close_files(Path input, Path output, Path error) = 0;

        #ifdef ON_WINDOWS
            virtual ReturnState __run_child_process_program(Path input_file, Path output_file, Path error_file, int time_limit, _enum::_FuncProgramType func_type) = 0;
        #else
            ReturnState __run_child_process_program(Path input_file, Path output_file, Path error_file, int time_limit, _enum::_FuncProgramType func_type) {
                auto start_time = tools::now();
                pid_t pid = fork();         
                if (pid == 0) {
                    __set_default_args();
                    int exit_code = __run_child_program(input_file, output_file, error_file, func_type);
                    __close_files(input_file, output_file, error_file);
                    _exit(exit_code == 0 ? 0 : -1);
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
                        while (result == 0 && tools::duration_ms(start_time) < limit.count()) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            result = waitpid(pid, &status, WNOHANG);
                        }     
                        if (result == 0) {
                            __terminate_process(reinterpret_cast<void*>(pid));
                        }   
                        result = waitpid(pid, &status, WNOHANG);              
                    }
         
                    int exit_status = WEXITSTATUS(status);
                    if (WIFEXITED(status) && exit_status == -1) {     
                        _msg::__warn_msg(_msg::_defl, "Fail to run program or something error.");
                        return {_setting::_error_return, -1};
                    } else {
                        return {exit_status, (int)tools::duration_ms(start_time)};   
                    }    
                         
                } 
                else {
                    _msg::__warn_msg(_msg::_defl, "Fail to fork.");
                    return {_setting::_error_return, -1};
                }            
            }    
        #endif
        public:
            virtual bool __check_program_valid() = 0;

            template<typename T1, typename T2, typename T3>
            typename std::enable_if<
                IsPathConstructible<T1>::value &&
                IsPathConstructible<T2>::value &&
                IsPathConstructible<T3>::value, ReturnState>::type
            __run_program(T1 input_file, T2 output_file, T3 error_file, int time_limit, _enum::_FuncProgramType func_type) {
                if (!__check_program_valid()) return {_setting::_error_return, -1};
                Path input = Path(input_file);
                if (!input.__file_exist()) {
                    _msg::__error_msg(_msg::_defl, tools::string_format("Fail to open input file %s.", input.cname()));
                    return {_setting::_error_return, -1};
                }
                Path output = Path(output_file);
                output.__ensure_file_exist();
                Path error = Path(error_file);
                error.__ensure_file_exist();
                return __run_child_process_program(input, output, error, time_limit, func_type);
            }

            virtual _Program* __clone() = 0;
            virtual _Program* __clone(const std::string& args) = 0;

            _SET_GET_VALUE(std::string, args)
            template <typename...Args>
            void add_args(const Args&... others) {
                _args = tools::string_join(" ", _args, others...);
            }         
            void clear_args() { _args.clear(); }
            _SET_GET_VALUE(bool, enable_default_args)
            _SET_GET_VALUE(std::string, name)
        };

    } // namespace io
} // namespace generator

#endif // !_SGPCET_PROGRAM_H_
