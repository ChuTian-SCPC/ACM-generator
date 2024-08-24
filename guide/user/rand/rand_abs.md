## rand_abs

1. `double rand_abs()`

   返回一个浮点数类型为`double`，范围为 $(-1,1)$ 。

2. `T rand_abs(T from)` 

   传入一个类型为`T`的参数 $n$ ，`T`需要为整型或者浮点型。

   返回一个类型为`T`的数 $x$ 。

   如果`T`为整型，那么 $x$ 的范围是 $[-n,n]$ ;

   如果`T`为浮点型，那么$x$的范围是 $(-n, n)$ 。

3. `long long rand_abs(T from, U to)`

   传入两个类型分别为`T,U`的参数 $from, to$ ，`T,U`均需要为整型。

   返回一个整数类型为`long long`，范围为 $[-to,-from]\cup[from, to]$ 。

4. `double rand_abs(T from, U to)`

   传入两个类型分别为`T,U`的参数 $from, to$ ，`T,U`均需要为可以转换成浮点型的类型，且`T,U`不都是整型。

   返回一个浮点数类型为`double`，范围为 $[-to, -from]\cup[from, to)$ 。

5. `T rand_abs(const char* format,...)`

   传入一个类似`printf`中的格式控制字符串。

   返回一个数类型为`T`，`T`需要为整型或者浮点型，默认为`double`，值绝对值的范围为格式控制字符串所描述。

   格式控制字符串示例可见[rand_int](./rand_int.md)和[rand_real](./rand_real.md)

[示例代码](../../../examples/rand_abs.cpp)

[返回目录](../../home.md)
