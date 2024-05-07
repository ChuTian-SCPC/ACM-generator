## rand_char

内置常用的字符集：[CharType](./char_type.md)

1. `char rand_char(CharType type = LowerLetter)`

   传入一个类型为`CharTyoe`的参数 $type$ ，默认为 `LowerLetter`（小写字母）。

   返回一个字符，字符集由参数 $type$ 所描述。

2. `char rand_char(const char* format,...)`

   传入一个`testlib.h`中的极简正则表达式。
   
   返回一个字符，字符集由该正则表达式所描述。
   
   如果传入的正则表达式是生成一个字符串。
   
   即使传入的正则表达式生成的字符串长度超过 $1$ ，也**只会**返回第一个字符。
   
   如果传入的正则表达式生成的字符串长度为 $0$ （空字符串），那么会触发数据生成错误。

3. `char rand_char(string format)`
   
   传入一个`testlib.h`中的极简正则表达式。
   
   返回一个字符，字符集由该正则表达式所描述。
   
   如果传入的正则表达式是生成一个字符串。
   
   即使传入的正则表达式生成的字符串长度超过 $1$ ，也**只会**返回第一个字符。
   
   如果传入的正则表达式生成的字符串长度为 $0$ （空字符串），那么会触发数据生成错误。

[示例代码](../../../examples/rand_char.cpp)

[返回目录](../../home.md)
