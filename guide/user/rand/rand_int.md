## rand_int

1. `T rand_int(T n)` 

   传入一个类型为`T`的参数 $n$ ，`T`需要为整型。

   返回一个整数类型为`T`，范围为 $[0,n)$ 。

2. `T rand_int(T from, T to)` 

   传入两个类型为`T`的参数 $from, to$ ，`T`需要为整型。

   返回一个整数类型为`T`，范围为 $[from, to]$ 。

3. `long long rand_int(T from, U to)` 

   传入两个类型分别为`T,U`的参数 $from, to$ ，`T,U`均需要可以转化为整型。

   返回一个整数类型为`long long`，范围为 $[from, to]$ 。

   注意，如果$from,to$的大小如果超过`long long`的范围话，那么会出现问题。

4. `T rand_int(const char* format,...)` 

   传入一个类似`printf`中的格式控制字符串。

   返回一个整数类型为`T`，`T`必须为整型，默认为`long long`，范围为格式控制字符串所描述。

   格式控制字符串中应该包含一个左括号'['或'('，一个右括号']'或')'表示开闭区间，以及一个逗号分隔左右端点。

   例子：

   ```cpp
   rand_int("[3, 5]");
   rand_int<int>("(%d, %lld)", 3, 5LL);
   rand_int<unsigned int>("[%s, 6)", "3");
   rand_int(1, 1e9);
   ```

现在`rand_int`支持`unsigned int`, `unsigned long`, `unsigned long long`这些类型的生成。这在`testlib`中是不支持的，详见[示例代码](../../../examples/rand_int_unsigned.cpp)。

[示例代码](../../../examples/rand_int.cpp)

[返回目录](../../home.md)
