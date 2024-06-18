## 森林(Forest)

生成一个森林。

构造函数是：
```cpp
// 无权
Forest(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1
);

// 带点权
Forest<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
Forest<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
Forest<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

森林是无向，无重边，无自环的图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
bool& direction_ref();
void set_multiply_edge(bool multiply_edge);
bool& multiply_edge_ref();
void set_self_loop(bool self_loop);
bool& self_loop_ref();
```

森林的连通性由边数决定。

森林中每个树的大小为`trees_size`。

可以通过以下函数去设置：

```cpp
void add_tree_size(int tree_size);
void set_trees_size(std::vector<int> trees_size);
```
可以通过以下函数去获取：

```cpp
std::vector<int> trees_size() const;
std::vector<int>& tree_size_ref();
```

如果`trees_size`和`node_count`与`edge_count`不符合，会修改相关的值。

如果`trees_size`没有设置，会随机设置。

[示例代码](../../../examples/forest.cpp)：
生成一棵 $10$ 个结点， $7$ 条边的森林，即 $3$ 棵树组成。


[返回上级](./summary.md)

[返回目录](../../home.md)