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
            std::map<Name, std::vector<int>> _compares;
            std::set<Name> _hacks;

            Time _time_limit;
            Time _time_limit_for_std;
            Time _time_limit_for_generate;
            Time _time_limit_for_check;
            Time _time_limit_for_validate;
            bool _limit_std_runtime;

            bool _delete_fail_testcase;

            // input
            bool _fill_input_with_exist;

            // output
            bool _cover_exist;

            // hack
            bool _max_try;
            bool _stop_when_wrong;
            bool _copy_wrong_to_testcase;
            bool _delete_correct;

            // log
            bool _detail_report_on_console;
            bool _detail_report_on_file;
        public:
            
        };
    } // namespace io
} // namespace generator

#endif // _SGPCET_WORKFLOW_H_
