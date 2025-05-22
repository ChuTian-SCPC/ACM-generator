#ifndef _SGPCET_CHAR_SET_CHECK_H_
#define _SGPCET_CHAR_SET_CHECK_H_

#ifndef _SGPCET_SETTING_H_
#include "basic/setting.h"
#endif //!_SGPCET_SETTING_H_
#ifndef _SGPCET_COLOR_H_
#include "log/color.h"
#endif //!_SGPCET_COLOR_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif //!_SCPCET_LOGGER_H_
#ifndef _SGPCET_TOOLS_H_
#include "basic/tools.h"
#endif //!_SGPCET_TOOLS_H_

namespace generator {
    namespace _setting {
        class CharSetCheck {
        private:
            static _msg::_ColorMsg _func;
        public:
            CharSetCheck() = default;
            static void enable_default_write() {
                if (_setting::BigNumberSetting::has_empty_or_same_label()) {
                    _msg::__fail_msg(_msg::_defl, 
                        "Can't use default write when BigNumberSetting has empty or same labels.\n",
                        "Using ", _func, " to see the detail.");
                }
            }
            static void enable_default_read() {
                if (_setting::BigNumberSetting::has_empty_or_same_label() && !_setting::BigNumberSetting::is_single_label()) {
                    _msg::__fail_msg(_msg::_defl, 
                        "Can't use default read when BigNumberSetting has multiply labels, has empty labels or has same labels.\n",
                        "Using ", _func, " to see the detail.");
                }
            }

            static void enable_base(int base) {
                if (base <= 1) {
                    _msg::__fail_msg(_msg::_defl, "base must greater or equal to 2, but found %d.", base);
                }
                int size = _setting::BigNumberSetting::labels().size();
                if (size < base) {
                    _msg::__fail_msg(_msg::_defl, tools::string_format(
                        "BigNumberSetting can only represent numbers in up to base %d, but require is %d.", size, base));
                }
            }

            static void detail() {
                std::map<std::string, std::vector<int>> same;
                std::vector<std::pair<int, std::string>> nsingle;
                std::vector<int> empty;
                bool has_same = false;
                auto& labels = _setting::BigNumberSetting::labels();
                _msg::__info_msg(_msg::_defl, tools::string_format(
                    "The BigNumberSetting can represent numbers in up to base %d.", labels.size()));
                for (int i = 0; i < labels.size(); i++) {
                    auto label = labels[i];
                    if (label.empty()) empty.emplace_back(i);
                    else {
                        if (label.size() >= 2) nsingle.emplace_back(i, label);
                        same[label].emplace_back(i);
                        if (same[label].size() >= 2) has_same = true;
                    }
                }
                if (has_same) {
                    _msg::__endl(_msg::_defl);
                    _msg::__info_msg(_msg::_defl, "There are character(s) in the BigNumberSetting that represent multiple digits:");
                    for (auto& s : same) {
                        if (s.second.size() >= 2) {
                            _msg::__info_msg(_msg::_defl, tools::string_format(
                                "%s : %s", s.first.c_str(), join(s.second.begin(), s.second.end(), ", ").c_str()));
                        }
                    }   
                }
                if (empty.size() >= 1) {
                    _msg::__endl(_msg::_defl);
                    _msg::__info_msg(_msg::_defl, "There are empty string in the BigNumberSetting that represent digits:");
                    _msg::__info_msg(_msg::_defl, join(empty.begin(), empty.end(), ", ") );
                }
                if (nsingle.size() >= 1) {
                    _msg::__endl(_msg::_defl);
                    _msg::__info_msg(_msg::_defl, "There are strings in the BigNumberSetting that are not single characters:"); 
                    for (auto& s : nsingle) {
                        _msg::__info_msg(_msg::_defl, tools::string_format(
                            "%d : %s", s.first, s.second.c_str()));
                    }
                }

            }

        };

        _msg::_ColorMsg CharSetCheck::_func = _msg::_ColorMsg("CharSetCheck::detail()", _enum::Color::Green);
        
    } // namespace _setting
} // namespace generator

#endif //!_SGPCET_CHAR_SET_CHECK_H_
