### 无权边

边由`class Edge`进行存储。

可以通过函数获取边的结点以及修改：

```cpp
int &u()                    //获取第一个结点
int &v()                    //获取第二个结点
std::tuple<int, int> edge() //获取由tuple包裹的u,v
```

可以通过以下方式进行输出：

```cpp
std::ostream &operator<< //流输出，格式为"u v"(没有换行)
print()                  //没有换行
println()                //有换行
```

其中，`print`和`println`还可以通过传入一个`const char*`或`std::string`类型的格式字符串`format`进行输出。

`format`的格式要求是`%u`输出 $u$ , `%v` 输出 $v$ ，其余直接输出。可以看[相关示例](../../../examples/print_edge.cpp)

[返回上级](./summary.md)

[返回目录](../../home.md)
