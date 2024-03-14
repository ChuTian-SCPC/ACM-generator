## unweight::WheelGraph

生成一个轮图。

构造函数是：
```cpp
WheelGraph(
    int node = 4, 
    int begin_node = 1
)；
```

轮图是无重边，无自环，一定连通，边数一定的图，所以以下函数被禁用：
```cpp
void set_multiply_edge();
void set_connect();
void set_self_loop();
void set_side(int side);
void set_side_count(int count);
void set_edge_count(int count);
```

[示例代码](../../../examples/unweight_wheel_graph.cpp)：
生成一棵 $5$ 个结点的轮图。
输出时不输出边数。

[返回上级](./summary.md)

[返回目录](../../home.md)