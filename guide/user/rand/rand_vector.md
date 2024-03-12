## rand_vector

1. `vector<T> rand_vector(int size,Func func)`
   传入一个类型为`int`的参数 $size$ 和一个类型为 `Func` 的参数 `func` 。

   返回一个数组类型为`vector<T>`，长度为 $size$ ，数组元素由函数 `func()` 随机生成。
2. `vector<T> rand_vector(int from,int to,Func func)`
   传入两个类型为`int`的参数 $from,to$ 和一个类型为 `Func` 的参数 `func` 。

   返回一个数组类型为`vector<T>`，长度在区间 $[from,to]$ 内，数组元素由函数 `func()` 随机生成。

[示例代码](../../../examples/rand_vector.cpp)

[返回目录](../../home.md)