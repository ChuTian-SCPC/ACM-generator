## 网格图(GridGraph)

生成一个网格图。

构造函数是：
```cpp
// 无权
GridGraph(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    int row = -1
);

// 带点权
GridGraph<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    int row = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
GridGraph<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    int row = -1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
GridGraph<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1,
    int row = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

网格图是无自环的图，所以以下函数被禁用：
```cpp
void set_self_loop(bool self_loop);
bool& self_loop_ref();
```

网格图的行数是`row`，列数是`column`。

可以通过以下函数去设置：

```cpp
void set_row(int row);                                   // column = (node_count + row - 1) / row
void set_column(int column);                             // row = node_count + column - 1) / column
void set_row_column(int row, int column, int ignore = 0);// nrow * column - ignore
```
可以通过以下函数去获取：

```cpp
int row() const;
int column() const;

int row_ref();
int column_ref();
```



如果不设置，`row`的默认值为 $-1$ ，行数采取随机一个值。

由于边数上界计算较困难，如果在随机的情况下，边数大于上界的情况会采用上界进行生成。

[示例代码1](../../../examples/grid_graph1.cpp)：
生成一棵 $9$ 个结点， $10$ 条边，行数为 $2$ 的网格图。
它要保证一定连通。

[示例代码2](../../../examples/grid_graph2.cpp)：
生成一棵 $9$ 个结点， $100$ 条边的网格图。
由于行数随机，所以会选取边数的上界情况 $12$ 进行生成。

[返回上级](./summary.md)

[返回目录](../../home.md)