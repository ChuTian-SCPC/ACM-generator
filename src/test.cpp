#include "log/logger.h"
using namespace std;
using namespace generator::_msg;
using namespace generator::io;
using namespace generator::_setting;
int main() {
  OutStream out;
  out.println("123", string_format("%c", 'a'), _ColorMsg("789", Color::Red));
  out.println(_checker_folder);
  return 0;
}