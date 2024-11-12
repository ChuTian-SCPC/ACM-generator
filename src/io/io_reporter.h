#ifndef _SGPCET_IO_REPORTER_H_
#define _SGPCET_IO_REPORTER_H_

#ifndef _SGPCET_ENUM_H_
#include "common/enum.h"
#endif // !_SGPCET_ENUM_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_IO_INIT_H_
#include "io/init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {

        const char* __stage_name(_enum::_Stage stage) {
            if (stage == _enum::_INPUT) return "Make input";
            if (stage == _enum::_OUTPUT) return "Make output";
            if(stage == _enum::_VALID) return "Check input";
            return "UNKNOWN";
        }

        const char* __stage_file(_enum::_Stage stage, int x) {
            Path path = stage == _enum::_OUTPUT ? __output_file_path(x) : __input_file_path(x);
            return path.cname();
        }

        void __report_iov_summary_logs(std::unordered_map<int, bool>& results, _enum::_Stage stage) {
            int count = 0;
            std::vector<int> errors;
            for (auto result : results) {
                count++;
                if (!result.second) errors.emplace_back(result.first);
            }
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Summary", _enum::Color::Green), " :");
            _msg::__info_msg(_msg::_defl, tools::string_format("%s files (success / all) : %d / %d.", __stage_name(stage), count - errors.size(), count));
            if (errors.size()) {
                _msg::__info_msg(_msg::_defl, "Program meets errors in files :");
                for (int i : errors) 
                    _msg::__info_msg(_msg::_defl, tools::string_format("  %s", __stage_file(stage, i)));
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Please Check.", _enum::Color::Red));
            } else {
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("All Success.", _enum::Color::Green));
            }
            _msg::__endl(_msg::_defl);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_IO_REPORTER_H_