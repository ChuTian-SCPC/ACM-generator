#ifndef _SGPCET_IO_INIT_H_
#define _SGPCET_IO_INIT_H_

#ifndef _SGPCET_PROGRAM_H_
#include "program.h"
#endif // !_SGPCET_PROGRAM_H_

namespace generator {
    namespace io {
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
    } // namespace generator
} // namespace io

#endif // !_SGPCET_IO_INIT_H_
