## 仙人掌(Cactus)

生成一个仙人掌。

构造函数是：
```cpp
// 无权
Cactus(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1
);

// 带点权
Cactus<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
Cactus<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
Cactus<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

仙人掌是无重边，无自环，一定连通的无向图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
bool& direction_ref();
void set_multiply_edge(bool multiply_edge);
bool& multiply_edge_ref();
void set_self_loop(bool self_loop);
bool& self_loop_ref();
void set_connect(bool connect);
bool& connect_ref();
```

[示例代码](../../../examples/cactus.cpp)：
生成一棵 $9$ 个结点, 边数为 $12$ 的仙人掌。

[返回上级](./summary.md)

[返回目录](../../home.md)