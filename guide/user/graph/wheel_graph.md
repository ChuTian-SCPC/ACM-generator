## 轮图(WheelGraph)

生成一个轮图。

构造函数是：
```cpp
// 无权
WheelGraph(
    int node_count = 1, 
    int begin_node = 1
);

// 带点权
WheelGraph<NodeType>(
    int node_count = 1, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
WheelGraph<EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
WheelGraph<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

轮图是无重边，无自环，一定连通，边数一定的图，所以以下函数被禁用：
```cpp
void set_multiply_edge(bool multiply_edge);
bool& multiply_edge_ref();
void set_connect(bool connect);
bool& connect_ref();
void set_self_loop(bool self_loop);
bool& self_loop_ref();
void set_edge_count(int count);
int& edge_count_ref();
```

[示例代码](../../../examples/wheel_graph.cpp)：
生成一棵 $5$ 个结点的轮图。
输出时不输出边数。

[返回上级](./summary.md)

[返回目录](../../home.md)