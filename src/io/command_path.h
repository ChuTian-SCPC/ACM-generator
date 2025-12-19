#ifndef _SGPCET_COMMAND_PATH_H_
#define _SGPCET_COMMAND_PATH_H_

#ifndef _SGPCET_PROGRAM_H_
#include "program.h"
#endif // !_SGPCET_PROGRAM_H_

namespace generator {
    namespace io {
        class CommandPath : public _Program {
        protected:
            Path _path;
        public:
            CommandPath() : _path(Path()), _Program() {}
            CommandPath(CommandPath&& other) noexcept : _Program(std::move(other)), _path(std::move(other._path)) {}
            CommandPath(const CommandPath& other) : _Program(other), _path(other._path) {}
            CommandPath& operator=(const CommandPath& other) {
                if (this != &other) {
                    _Program::operator=(other);
                    _path = other._path;
                }
                return *this;
            }
            CommandPath& operator=(CommandPath&& other) noexcept {
                if (this != &other) {
                    _Program::operator=(std::move(other));
                    _path = std::move(other._path);
                }
                return *this;
            }

            template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
            CommandPath(T&& s) : _path(std::forward<T>(s)), _Program() {}
            template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
            CommandPath(T&& s, std::string args) : _path(std::forward<T>(s)), _Program(args) {}

        protected:
            virtual std::string __default_name() override {
                return _path.__file_name();
            }
            virtual void __close_files(Path input, Path output, Path error) override {
                return;
            }

            std::string __command() { return tools::string_join(" ", _path.full_path(), _args);}

            virtual int __run_child_program(Path input, Path output, Path error, _enum::_FuncProgramType type) override {
                std::string command = __command();
                if (_enable_default_args) {
                   if (!input.__empty()) command = tools::string_join(" ", command, "<", input);
                   if (!output.__empty()) command = tools::string_join(" ", command, ">", output);
                   if (!error.__empty()) command = tools::string_join(" ", command, "2>", error);
                }
                int code = std::system(command.c_str());
                return code;
            }

            
        #ifdef ON_WINDOWS
            virtual ReturnState __run_child_process_program(Path input_file, Path output_file, Path error_file, int time_limit, _enum::_FuncProgramType func_type) override {
                __set_default_args();
                auto start_time = tools::now();
                SECURITY_ATTRIBUTES sa;
                sa.nLength = sizeof(sa);
                sa.lpSecurityDescriptor = NULL;
                sa.bInheritHandle = TRUE;       
                
                HANDLE hInFile = INVALID_HANDLE_VALUE;
                HANDLE hOutFile = INVALID_HANDLE_VALUE;
                HANDLE hErrorFile = INVALID_HANDLE_VALUE;

                if (!input_file.__empty()) {
                    if (!input_file.__file_exist())  return {_setting::_error_return, -1};
                    hInFile = CreateFileA(input_file.cname(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        &sa,
                        OPEN_EXISTING ,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL );
                }
                
                if (!output_file.__empty()) {
                    hOutFile = CreateFileA(output_file.cname(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_WRITE | FILE_SHARE_READ,
                        &sa,
                        CREATE_ALWAYS ,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL );                
                }

                if (!error_file.__empty()) {
                    hErrorFile = CreateFileA(error_file.cname(),
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
                ret = CreateProcessA(NULL, const_cast<char *>(__command().c_str()), NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
                if (ret) 
                {
                    if (__is_time_limit_inf(time_limit))  WaitForSingleObject(pi.hProcess, INFINITE);
                    else if (WaitForSingleObject(pi.hProcess, time_limit) == WAIT_TIMEOUT) __terminate_process(pi.hProcess);
                    DWORD exitCode;
                    GetExitCodeProcess(pi.hProcess, &exitCode);
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                    if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                    if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                    if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                    return {(int)exitCode, (int)tools::duration_ms(start_time)};
                }
                else {
                    if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                    if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                    if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                    return {_setting::_error_return, -1};
                }   
            }
        #endif
        
        public:
            _GET_VALUE(Path, path)
            template <typename T>
            typename std::enable_if<IsPathConstructible<T>::value, void>::type
            set_path(T path) {
                _path = Path(path);
            }

            virtual bool __check_program_valid() override {
                if (!_path.full_path().__file_exist()) {
                    _msg::__error_msg(_msg::_defl, tools::string_format("File %s not exist.", _path.cname()));
                    return false;
                }
                return true;
            }

            virtual _Program* __clone() override {
                return new CommandPath(*this);
            }

            virtual _Program* __clone(const std::string& args) override {
                _Program* program = new CommandPath(*this);
                program->add_args(args);
                return program;
            }
            
            virtual std::string __path_or_func() override {
                return _path.full_path().path();
            }

        };
    } // namespace io
} // namespace generator

#endif // _SGPCET_COMMAND_PATH_H_
