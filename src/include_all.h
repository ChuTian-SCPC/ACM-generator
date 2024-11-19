#ifndef _SGPCET_INCLUDE_ALL_H_
#define _SGPCET_INCLUDE_ALL_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_

#ifndef _SGPCET_SETTING_H_
#include "basic/setting.h"
#endif // !_SGPCET_SETTING_H_
#ifndef _SGPCET_TOOLS_H_
#include "basic/tools.h"
#endif // !_SGPCET_TOOLS_H_
#ifndef _SGPCET_ENUM_H_
#include "basic/enum.h"
#endif // !_SGPCET_ENUM_H_
#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
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
#ifndef _SGPCET_IO_INIT_H_
#include "io/io_init.h"
#endif // !_SGPCET_IO_INIT_H_
#ifndef _SGPCET_PROGRAM_H_
#include "io/program.h"
#endif // !_SGPCET_PROGRAM_H_
#ifndef _SGPCET_IO_REPORTER_H_
#include "io/io_reporter.h"
#endif // !_SGPCET_IO_REPORTER_H_
#ifndef _SGPCET_INPUTS_OUTPUTS_H_
#include "io/inputs_outputs.h"
#endif // !_SGPCET_INPUTS_OUTPUTS_H_
#ifndef _SGPCET_COMPARE_HACK_H_
#include "io/compare_hack.h"
#endif // !_SGPCET_COMPARE_HACK_H_
#ifndef _SGPCET_VALIDATE_H_
#include "io/validate.h"
#endif // !_SGPCET_VALIDATE_H_

#ifndef _SGPCET_NUMBER_CONST_H_
#include "rand/number_const.h"
#endif // !_SGPCET_NUMBER_CONST_H_
#ifndef _SGPCET_NUMBER_FORMAT_H_
#include "rand/number_format.h"
#endif // !_SGPCET_NUMBER_FORMAT_H_
#ifndef _SGPCET_NUMERIC_H_
#include "rand/numeric.h"
#endif // !_SGPCET_NUMERIC_H_
#ifndef _SGPCET_ARRAY_H_
#include "rand/array.h"
#endif // !_SGPCET_ARRAY_H_

#ifndef _SGPCET_EDGE_H_
#include "graph/edge.h"
#endif // !_SGPCET_EDGE_H_
#ifndef _SGPCET_NODE_H_
#include "graph/node.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_BASIC_TREE_H_
#include "graph/basic_tree.h"
#endif // !_SGPCET_BASIC_TREE_H_
#ifndef _SGPCET_WEIGHT_TYPE_H_
#include "graph/weight_type.h"
#endif // !_SGPCET_WEIGHT_TYPE_H_
#ifndef _SGPCET_GEN_FUNCTION_H_
#include "graph/gen_function.h"
#endif // !_SGPCET_GEN_FUNCTION_H_
#ifndef _SGPCET_LINK_FORWARD_H_
#include "graph/link_forward.h"
#endif // !_SGPCET_LINK_FORWARD_H_
#ifndef _SGPCET_GEN_STRATEGY_H_
#include "graph/gen_strategy.h"
#endif // !_SGPCET_GEN_STRATEGY_H_
#ifndef _SGPCET_TREE_H_
#include "graph/tree.h"
#endif // !_SGPCET_TREE_H_
#ifndef _SGPCET_UNWEIGHT_H_
#include "graph/unweight.h"
#endif // !_SGPCET_UNWEIGHT_H_
#ifndef _SGPCET_NODE_WEIGHT_H_
#include "graph/node_weight.h"
#endif // !_SGPCET_NODE_WEIGHT_H_
#ifndef _SGPCET_EDGE_WEIGHT_H_
#include "graph/edge_weight.h"
#endif // !_SGPCET_EDGE_WEIGHT_H_
#ifndef _SGPCET_BOTH_WEIGHT_H_
#include "graph/both_weight.h"
#endif // !_SGPCET_BOTH_WEIGHT_H_

namespace generator {
  namespace all {
    using namespace generator::tools;
    using namespace generator::_msg;
    using namespace generator::_setting;
    using namespace generator::_enum;
    using namespace generator::io;
    using namespace generator::rand_numeric;
    using namespace generator::rand_array;
    using namespace generator::rand_graph;
  }
} // namespace generator

#ifdef _SGPCET_MACRO_H_
#undef _GET_VALUE_CONST
#undef _GET_VALUE_REF
#undef _GET_VALUE
#undef _SET_VALUE
#undef _SET_GET_VALUE
#undef _OUTPUT_FUNCTION
#undef _OUTPUT_FUNCTION_SETTING
#undef _DEF_GEN_FUNCTION
#undef _CONTEXT_V
#undef _CONTEXT_V_REF
#undef _CONTEXT_GET
#undef _CONTEXT_GET_REF
#endif

#endif // !_SGPCET_INCLUDE_ALL_H_