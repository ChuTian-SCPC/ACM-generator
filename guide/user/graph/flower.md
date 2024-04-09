## 菊花图(Flower)

生成一个菊花图。

构造函数是：
```cpp
// 无权
Flower(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1
);

// 带点权
Flower<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
Flower<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
Flower<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

[示例代码](../../../examples/flower.cpp)：
生成一棵 $5$ 个结点的菊花图，结点起始编号为 $1$ 。

[返回上级](./summary.md)

[返回目录](../../home.md)

