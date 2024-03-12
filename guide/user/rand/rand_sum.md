## rand_sum

1. `vector<T> rand_sum(int size,T sum)`
   传入一个类型为`int`的参数 $size$ 和一个类型为 `T` 的参数 $sum$ 。

   返回一个**整型**数组类型为`vector<T>`，长度为 $size$ ，数组元素和为 $sum$ 。
2. `vector<T> rand_sum(int size,T sum,T min_part)`
   传入一个类型为`int`的参数 $size$ 和两个类型为 `T` 的参数 $sum, min\_part$ 。

   返回一个**整型**数组类型为`vector<T>`，长度为 $size$ ，数组元素和为 $sum$ , 且数组元素均不小于 $min\_part$ 。
3. `vector<T> rand_sum(int size,T sum,T from,T to)`
   传入一个类型为`int`的参数 $size$ 和三个类型为 `T` 的参数 $sum, from, to$ 。

   返回一个**整型**数组类型为`vector<T>`，长度为 $size$ ，数组元素和为 $sum$ , 且数组元素均在区间 $[from, to]$ 内。

[示例代码](../../../examples/rand_sum.cpp)

[返回目录](../../home.md)