## 图(Graph)

生成一个普通的图。

构造函数是：

```cpp
// 无权
Graph(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1
);

// 带点权
Graph<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
Graph<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
Graph<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

[示例代码1](../../../examples/graph1.cpp)：
生成一棵 $5$ 个点， $6$  条边的无向图，结点起始编号为 $0$ ，保证一定连通 。

[示例代码2](../../../examples/graph2.cpp)：
生成一棵 $4$ 个点， $10$  条边的有向图，结点起始编号为 $1$ ，可能存在重边。
输出的时候不输出它的边数。

[返回上级](./summary.md)

[返回目录](../../home.md)