## 链(Chain)

生成一条链。

构造函数是：
```cpp
// 无权
Chain(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1
);

// 带点权
Chain<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
Chain<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
Chain<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

[示例代码](../../../examples/chain.cpp)：
生成一棵 $5$ 个结点的链，结点起始编号为 $1$ 。

[返回上级](./summary.md)

[返回目录](../../home.md)

