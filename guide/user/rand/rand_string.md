## rand_string

内置常用的字符集：[CharType](./char_type.md)

1. `string rand_string(int n, CharType type = LowerLetter)` 

   传入一个整数 $n$ 和一个类型为`CharTyoe`的参数 $type$ ，默认为 `LowerLetter`（小写字母）。

   返回一个字符串长度为 $n$ ，字符集由参数 $type$ 所描述。

2. `string rand_string(int from, int to, CharType type = LowerLetter)` 

   传入两个整数 $from,to$ 和一个类型为`CharTyoe`的参数 $type$ ，默认为 `LowerLetter`（小写字母）。

   返回一个字符串长度在范围 $[from,to]$ 内，字符集由参数 $type$ 所描述。

3. `string rand_string(int n,const char *format,...)`

   传入一个整数 $n$ 和一个`testlib.h`中的极简正则表达式。

   返回一个字符串长度为 $n$ ，字符集由该正则表达式所描述。

4. `string rand_string(int from,int to,const char* format,...)`

   传入两个整数 $from,to$ 和一个`testlib.h`中的极简正则表达式。
   
   返回一个字符串长度在范围 $[from,to]$ 内，字符集由该正则表达式所描述。

[示例代码](../../../examples/rand_string.cpp)

[返回目录](../../home.md)