## 限制最大度数的树(DegreeTree)

生成一棵树，树上每个结点的度数不会超过限制。

构造函数是：
```cpp
// 无权
DegreeTree(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_degree = -1
);

// 带点权
DegreeTree<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_degree = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
DegreeTree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_degree = -1,
    EdgeGenFunction edges_weight_function = nullptr
);

// 带点权和边权
DegreeTree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_degree = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

树的最大度数是`max_degree`。

可以通过以下函数去设置：

```cpp
void set_max_degree(int max_degree);
bool& is_rooted_ref();
```

可以通过以下函数去获取：

```cpp
int max_degree() const;
int& max_degree_ref();
```

如果不设置，`max_degree`的默认值为 $-1$ ，树的最大度数采取随机值。

[示例代码](../../../examples/degree_tree.cpp)

生成一棵 $10$ 个结点的无根树，每个结点的度数不超过 $3$ 。

[返回上级](./summary.md)

[返回目录](../../home.md)