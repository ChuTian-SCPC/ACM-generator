## rand_even

1. `T rand_even(T n)`

   传入一个类型为`T`的参数 $n$ ，`T`需要为整型。

   返回一个整数类型为`T`，范围为 $[0,n]$ ，且为偶数。

2. `long long rand_even(T from, U to)`

   传入两个类型分别为`T,U`的参数 $from, to$ ，`T,U`均需要为整型。

   返回一个整数类型为`long long`，范围为 $[from, to]$ ，且为偶数。

   注意，如果 $from,to$ 的大小如果超过`long long`的范围话，那么会出现问题。

3. `T rand_even(const char* format,...)`

   传入一个类似`printf`中的格式控制字符串。

   返回一个整数类型为`T`，`T`必须为整型，默认为`long long`，范围为格式控制字符串所描述，且为偶数。

   格式控制字符串示例可见[rand_int](./rand_int.md)。

[示例代码](../../../examples/rand_even.cpp)

[返回目录](../../home.md)
