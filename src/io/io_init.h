#ifndef _SGPCET_IO_INIT_H_
#define _SGPCET_IO_INIT_H_

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

        Path __input_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _enum::_IN));
        }
        
        Path __output_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _enum::_OUT));
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __generator_program(T program, int x) {
            if (program.enable_default_args()) program.add_args(_setting::default_stable_seed + std::to_string(x));
            return program;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __generator_program(T program, int x) {
            std::string args = _setting::default_seed ? _setting::default_stable_seed + std::to_string(x) : "";
            return CommandFunc(program, args);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __generator_program(T program, int x) {
            std::string args = _setting::default_seed ? _setting::default_stable_seed + std::to_string(x) : "";
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
            static constexpr bool value = std::is_same<T, Checker>::value;
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

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __checker_porgram(T program) {
            return program;
        }

        template<typename T>
        typename std::enable_if<IsDefaultChecker<T>::value, CommandPath>::type
        __checker_porgram(T program) {
            CommandPath func(__get_default_checker_file(program));
            return func;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __checker_porgram(T program) {
            CommandFunc func(program);
            return func;
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __checker_porgram(T program) {
            CommandPath func(program);
            return func;
        }

        Path __compare_folder() {
            return __path_join(__current_path(), _setting::compare_folder);
        }

        bool __is_time_limit_inf(int time_limit) {
            return time_limit == _setting::time_limit_inf;
        }
        
        bool __time_limit_exceed(int time, int time_limit) {
            return !__is_time_limit_inf(time) && time > time_limit;
        }

        int __time_limit_extend(int time_Limit) {
            if (__is_time_limit_inf(time_Limit)) return time_Limit;
            return time_Limit * _setting::time_limit_over_ratio;
        }
    } // namespace generator
} // namespace io

#endif // !_SGPCET_IO_INIT_H_
