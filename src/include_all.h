#ifndef _SGPCET_INCLUDE_ALL_H_
#define _SGPCET_INCLUDE_ALL_H_

#ifndef _SGPCET_COMMON_H_
#include "common/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_SETTING_H_
#include "common/setting.h"
#endif // !_SGPCET_SETTING_H_
#ifndef _SGPCET_TOOLS_H_
#include "common/tools.h"
#endif // !_SGPCET_TOOLS_H_
#ifndef _SGPCET_ENUM_H_
#include "common/enum.h"
#endif // !_SGPCET_ENUM_H_
#ifndef _SGPCET_MACRO_H_
#include "common/macro.h"
#endif // !_SGPCET_MACRO_H_

#ifndef _SGPCET_COLOR_H_
#include "log/color.h"
#endif // !_SGPCET_COLOR_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_

#ifndef _SGPCET_PATH_H_
#include "io/path.h"
#endif // !_SGPCET_PATH_H_
#ifndef _SGPCET_COMMAND_PATH_H_
#include "io/command_path.h"
#endif // !_SGPCET_COMMAND_PATH_H_
#ifndef _SGPCET_COMMAND_FUNC_H_
#include "io/command_func.h"
#endif // !_SGPCET_COMMAND_FUNC_H_
#ifndef _SGPCET_PROGRAM_H_
#include "io/program.h"
#endif // !_SGPCET_PROGRAM_H_
#ifndef _SGPCET_IO_REPORTER_H_
#include "io/io_reporter.h"
#endif // !_SGPCET_IO_REPORTER_H_
#ifndef _SGPCET_INPUTS_OUTPUTS_H_
#include "io/inputs_outputs.h"
#endif // !_SGPCET_INPUTS_OUTPUTS_H_


namespace generator {
  namespace all {
    using namespace generator::tools;
    using namespace generator::_msg;
    using namespace generator::io;
    using namespace generator::_setting;
    using namespace generator::_enum;
  }
} // namespace generator

#ifdef _SGPCET_MACRO_H_
#undef _GET_VALUE_CONST
#undef _GET_VALUE_REF
#undef _GET_VALUE
#undef _SET_VALUE
#undef _SET_GET_VALUE
#endif

#endif // !_SGPCET_INCLUDE_ALL_H_