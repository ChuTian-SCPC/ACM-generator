## 基环外向树(PseudoOutTree)

生成一个基环外向树。

构造函数是：
```cpp
// 无权
PseudoOutTree(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1
);

// 带点权
PseudoOutTree<NodeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
PseudoOutTree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
PseudoOutTree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1,
    int cycle = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

基环外向树是无重边，无自环，一定连通，边数一定的有向图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
bool& direction_ref();
void set_multiply_edge(bool multiply_edge);
bool& multiply_edge_ref();
void set_self_loop(bool self_loop);
bool& self_loop_ref();
void set_connect(bool connect);
bool& connect_ref();
void set_edge_count(int count);
int& edge_count_ref();
```

环的大小是`cycle`。

可以通过以下函数去设置：

```cpp
void set_cycle(int cycle)
```

可以通过以下函数去获取：

```cpp
int cycle() const;
int cycle_ref();
```

如果不设置，`cycle`的默认值为 $-1$ ，环的大小采取随机一个值。

[示例代码](../../../examples/pseudo_out_tree.cpp)：
生成一棵 $7$ 个结点, 环大小为 $4$ 的基环外向树。

[返回上级](./summary.md)

[返回目录](../../home.md)