#ifndef _SGPCET_WORKFLOW_H_
#define _SGPCET_WORKFLOW_H_

#ifndef _SGPCET_INPUT_H_
#include "input.h"
#endif // !_SGPCET_INPUT_H_
#ifndef _SGPCET_OUTPUT_H_
#include "output.h" 
#endif // !_SGPCET_OUTPUT_H_
#ifndef _SGPCET_VALIDATE_H_
#include "validate.h"
#endif // !_SGPCET_VALIDATE_H_
#ifndef _SGPCET_CHECKER_H_
#include "checker.h"
#endif // !_SGPCET_CHECKER_H_
#ifndef _SGPCET_COMPARE_H_
#include "compare.h"
#endif // !_SGPCET_COMPARE_H_
#ifndef _SGPCET_HACK_H_
#include "hack.h"
#endif // !_SGPCET_HACK_H_

namespace generator {
    namespace io {
        class Workflow {
        protected:
            using Name = std::string;
            using TestCase = std::pair<Name, int>;
            using Time = int;

            std::map<Name, _Program*> _programs;
            Name _std;
            Name _checker;
            Name _validator;
            std::vector<TestCase> _test_cases;
            
            _Input _input;
            _Compare _compare;
            _Hack _hack;

            Time _time_limit;
            Time _time_limit_for_std;
            Time _time_limit_for_generator;
            Time _time_limit_for_checker;
            Time _time_limit_for_validator;

            bool _delete_fail_testcase;

            // output
            bool _cover_exist;

            // hack
            bool _copy_wrong_to_testcase;
            bool _delete_correct;

            // log
            bool _detail_report_on_console;
            bool _detail_report_on_file;
            _msg::OutStream _file;

            enum class State {
                UNKNOWN,
                GEN_FAIL,
                VAL_FAIL,
                STD_TLE,
                STD_RE,
                SUCCESS
            };

            std::map<int, State> _states;
        public:
            Workflow(int time_limit = _setting::time_limit_inf) :
                _std(_setting::_empty_program_name),
                _checker("lcmp"),
                _validator(_setting::_empty_program_name),
                _input(),
                _compare(),
                _hack(),
                _time_limit(time_limit),
                _time_limit_for_std(_setting::time_limit_inf),
                _time_limit_for_generator(_setting::time_limit_inf),
                _time_limit_for_checker(_setting::time_limit_inf),
                _time_limit_for_validator(_setting::time_limit_inf),
                _delete_fail_testcase(true),
                _cover_exist(true),
                _copy_wrong_to_testcase(true),
                _delete_correct(true),
                _detail_report_on_console(false),
                _detail_report_on_file(true),
                _file(Path("./summary.log"))
            {
                __init_checkers();
            }
            
            ~Workflow() {
                for (auto& name : _programs) {
                    if (name.second != nullptr) delete name.second;
                }
            }

            _GET_VALUE(Name, std)
            _GET_VALUE(Name, checker)
            _GET_VALUE(Name, validator)
            _SET_GET_VALUE(Time, time_limit)
            _SET_GET_VALUE(Time, time_limit_for_std)
            _SET_GET_VALUE(Time, time_limit_for_generator)
            _SET_GET_VALUE(Time, time_limit_for_checker)
            _SET_GET_VALUE(Time, time_limit_for_validator)
            _SET_GET_VALUE(bool, delete_fail_testcase)
            _SET_GET_VALUE(bool, cover_exist)
            _SET_GET_VALUE(bool, copy_wrong_to_testcase)
            _SET_GET_VALUE(bool, delete_correct)
            _SET_GET_VALUE(bool, detail_report_on_console)
            _SET_GET_VALUE(bool, detail_report_on_file)

            template<typename T>
            void add_program(T&& program) {
                __add_program(std::forward<T>(program));
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            add_program(N&& name, T&& program) {
                __add_program(std::string(std::forward<N>(name)), std::forward<T>(program));
            }

            template<typename T>
            void set_std(T&& std) {
                _Program* program = __find_program(std::forward<T>(std));
                _std = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_std(N&& name, T&& std) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(std));
                _std = program->name();
            }

            template<typename T>
            void set_checker(T&& checker) {
                _Program* program = __find_program(std::forward<T>(checker));
                _checker = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_checker(N&& name, T&& checker) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(checker));
                _checker = program->name();
            }

            template<typename T>
            void set_validator(T&& validator) {
                _Program* program = __find_program(std::forward<T>(validator));
                _validator = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_validator(N&& name, T&& validator) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(validator));
                _validator = program->name();
            }

            void debug() {
                for(auto& p : _programs) {
                    std::cout << p.first << " " << p.second << " " << p.second->name() << std::endl;
                }
            }

        protected:
            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value, _Program*>::type
            __add_program(const Name& name, T&& program) {
                _Program* p = __program(std::forward<T>(program));
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                p->set_name(name);
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __add_program(const Name& name, T&& program) {
                _Program* p = __checker_program(std::forward<T>(program));
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                p->set_name(name);
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value, _Program*>::type
            __add_program(T&& program) {
                _Program* p = __program(std::forward<T>(program));
                Name name = p->name();
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __add_program(T&& program) {
                _Program* p = __checker_program(std::forward<T>(program));
                Name name = p->name();
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                _programs[name] = p;
                return p;
            }

            void __init_checkers() {
                for (int i = _enum::Checker::lcmp; i < _enum::Checker::MaxChecker; i++) {
                    __add_program((_enum::Checker)i);
                }
            }

            _Program* __find_program_by_name(const Name& name) {
                if (_programs.find(name) == _programs.end()) return nullptr;
                return _programs[name];
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __find_program(T&& index) {
                _Program* p = __find_program_by_name(checker_name[index]);
                if (p != nullptr) return p;
                __init_checkers();
                return __find_program_by_name(checker_name[index]);
            }

            template<typename T>
            typename std::enable_if<std::is_convertible<T, std::string>::value, _Program*>::type
            __find_program(T&& name) {
                _Program* p = __find_program_by_name(std::string(name));
                if (p != nullptr) return p;
                return __add_program(std::forward<T>(name));
            }
            
            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value && !std::is_convertible<T, std::string>::value, _Program*>::type
            __find_program(T&& name) {
                return __add_program(std::forward<T>(name));
            }

        };
    } // namespace io
} // namespace generator

#endif // _SGPCET_WORKFLOW_H_
