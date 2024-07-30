## rand_bracket_seq

1. `std::string rand_bracket_seq(int len, std::string brackets = "()")`
   
   传入一个整数 $len$ 和一个字符串 $brackets$ 。
   
   $len$ 需要为正偶数，字符串的长度也需要为偶数，两两作为一组括号匹配，默认为"()"。

   返回一个长度为 $len$ 的合法括号序列，它的括号字符集由 $brackets$ 决定。

2. `std::string rand_bracket_seq(int len, const char* format,...)`

    传入一个整数 $len$ 和一个`testlib.h`中的极简正则表达式。

    返回一个长度为 $len$ 的合法括号序列，它的括号字符集由该正则表达式所描述。

3. `std::string rand_bracket_seq(int from, int to, std::string brackets = "()")`

   传入两个整数 $from,to$ 和一个字符串 $brackets$ 。

   返回一个长度在范围 $[from,to]$ 内的合法括号序列，它的括号字符集由 $brackets$ 决定。

4. `std::string rand_bracket_seq(int from, int to, const char* format,...)`

   传入两个整数 $from,to$ 和一个字符串 $brackets$ 。

   返回一个长度在范围 $[from,to]$ 内的合法括号序列，它的括号字符集由该正则表达式所描述。

[示例代码](../../../examples/rand_bracket_seq.cpp)

[返回目录](../../home.md)