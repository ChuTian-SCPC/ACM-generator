## rand_palindrome

内置常用的字符集：[CharType](./char_type.md)

1. `string rand_palindrome(int n, int p, CharType type = LowerLetter)`
   
   传入两个整数 $n,p$ 和一个类型为`CharTyoe`的参数 $type$ ，默认为 `LowerLetter`（小写字母）。

   返回一个字符串长度为 $n$ ，它存在一个长度至少为 $p$ 的回文串，字符集由参数 $type$ 所描述。

2. `string rand_palindrome(int n, int p, const char* format, ...)`
   
   传入两个整数 $n,p$ 和一个`testlib.h`中的极简正则表达式。

   返回一个字符串长度为 $n$ ，它存在一个长度至少为 $p$ 的回文串，字符集由该正则表达式所描述。

3. `string rand_palindrome(int n, int p, string format)`
      
   传入两个整数 $n,p$ 和一个`testlib.h`中的极简正则表达式。

   返回一个字符串长度为 $n$ ，它存在一个长度至少为 $p$ 的回文串，字符集由该正则表达式所描述。
   

[示例代码](../../../examples/rand_palindrome.cpp)

[返回目录](../../home.md)