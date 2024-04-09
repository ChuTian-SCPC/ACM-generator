## 有向无环图(DAG)

生成一个有向无环图(DAG)。

构造函数是：
```cpp
// 无权
DAG(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1
);

// 带点权
DAG<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
DAG<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
DAG<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

DAG是有向无环的图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
void set_self_loop(bool self_loop);
```

[示例代码](../../../examples/dag.cpp)：
生成一棵 $5$ 个结点, $6$ 条边的DAG。

[返回上级](./summary.md)

[返回目录](../../home.md)