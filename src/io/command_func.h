#ifndef _SGPCET_COMMAND_FUNC_H_
#define _SGPCET_COMMAND_FUNC_H_

#ifndef _SGPCET_PROGRAM_H_
#include "program.h"
#endif // !_SGPCET_PROGRAM_H_

namespace generator {
    namespace io {
        template<typename T>
        struct IsFunctionConvertible {
            static constexpr bool value = std::is_convertible<T, std::function<void()>>::value;
        };

        class CommandFunc : public _Program {
        private:
            std::function<void()> _func;
        public:
            CommandFunc() : _func(nullptr), _Program() {}
            CommandFunc(CommandFunc&& other) noexcept : _func(other._func), _Program(std::move(other)) {}
            CommandFunc(const CommandFunc& other) : _func(other._func), _Program(other) {}
            CommandFunc& operator=(CommandFunc&& other) noexcept {
                if (this != &other) {
                    _Program::operator=(std::move(other));
                    _func = other._func;
                }
                return *this;
            }
            CommandFunc& operator=(const CommandFunc& other) {
                if (this != &other) {
                    _Program::operator=(other);
                    _func = other._func;
                }
                return *this;
            }

            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func) : _func(std::forward<T>(func)), _Program() {
                _name = __default_name();
            }
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func, std::string args) : _func(std::forward<T>(func)), _Program(args) {
                _name = __default_name();
            }


        protected:
            std::string __default_name() {
                std::string name = "function" + std::to_string(_setting::_function_count);
                _setting::_function_count++;
                return name;
            }

            virtual void __close_files(Path input, Path output, Path error) override {
                return;
            }

            virtual int __run_child_program(Path input, Path output, Path error, _enum::_FuncProgramType type) override {
                if (!input.__empty()) __open_input_file(input);
                if (!output.__empty()) __open_output_file(output);
                if (!error.__empty()) __open_error_file(error);
                __set_args(_args, type);
                _func();
                return EXIT_SUCCESS;
            }

        #ifdef ON_WINDOWS
            virtual ReturnState __run_child_process_program(Path input_file, Path output_file, Path error_file, int time_limit, _enum::_FuncProgramType func_type) override {
                __set_default_args();
                if (__is_time_limit_inf(time_limit)) {
                    auto start_time = tools::now();
                    int exit_code = __run_child_program(input_file, output_file, error_file, func_type);
                    __close_files(input_file, output_file, error_file);
                    return {exit_code, (int)tools::duration_ms(start_time)};
                } else {
                    _msg::__error_msg(_msg::_defl, "Unsupport running time limit function in Windows");
                    return {_setting::_error_return, -1};
                }
            }
        #endif // ON_WINDOWS
        
        public:
            _GET_VALUE(std::function<void()>, func)
            template <typename T>
            typename std::enable_if<IsFunctionConvertible<T>::value, void>::type
            set_func(T func) {
                _func = std::function<void()>(func);
            }

            virtual bool __check_program_valid() override {
                return true;
            }

            virtual _Program* __clone() override {
                return new CommandFunc(*this);
            }

            virtual _Program* __clone(const std::string& args) override {
                _Program* program = new CommandFunc(*this);
                program->add_args(args);
                return program;
            }

        };

        
    } // namespace io
    
} // namespace generator


#endif // !_SGPCET_COMMAND_FUNC_H_
