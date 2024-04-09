## 环图(CycleGraph)

生成一个环图。

构造函数是：
```cpp
// 无权
CycleGraph(
    int node_count = 1, 
    int begin_node = 1
);

// 带点权
CycleGraph<NodeType>(
    int node_count = 1, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
CycleGraph<EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
CycleGraph<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

环图是无重边，无自环，一定连通，边数一定的图，所以以下函数被禁用：
```cpp
void set_multiply_edge();
void set_connect();
void set_self_loop();
void set_edge_count(int count);
```

[示例代码](../../../examples/cycle_graph.cpp)：
生成一棵 $5$ 个结点的环图。
输出时不输出边数。

[返回上级](./summary.md)

[返回目录](../../home.md)