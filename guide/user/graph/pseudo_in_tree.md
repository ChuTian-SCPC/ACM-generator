

## 基环内向树(PseudoInTree)

生成一个基环内向树。

构造函数是：
```cpp
// 无权
PseudoInTree(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1
);

// 带点权
PseudoInTree<NodeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
PseudoInTree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
PseudoInTree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

基环内向树是无重边，无自环，一定连通，边数一定的有向图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
void set_multiply_edge(bool multiply_edge);
void set_self_loop(bool self_loop);
void set_connect(bool connect);
void set_edge_count(int count);
```

环的大小是`cycle`。

可以通过以下函数去设置：

```cpp
void set_cycle(int cycle)
```

可以通过以下函数去获取：

```cpp
int& cycle() ;
int ccycle() const ;
```

如果不设置，`cycle`的默认值为 $-1$ ，环的大小采取随机一个值。

[示例代码](../../../examples/pseudo_in_tree.cpp)：
生成一棵 $7$ 个结点, 环大小为 $4$ 的基环内向树。

[返回上级](./summary.md)

[返回目录](../../home.md)