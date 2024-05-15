## 二分图(BipartiteGraph)

生成一个二分图。

构造函数是：
```cpp
// 无权
BipartiteGraph(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1, 
    int left = -1
);

// 带点权
BipartiteGraph<NodeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1, 
    int left = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
BipartiteGraph<EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1, 
    int left = -1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
BipartiteGraph<NodeType, EdgeType>(
    int node_count = 1, 
    int edge_count = 0, 
    int begin_node = 1, 
    int left = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

二分图是无自环的无向图，所以以下函数被禁用：

```cpp
void set_direction(bool direction);
void set_self_loop(bool self_loop);
```

虽然是无向图，但是二分图的参数`swap_node`默认值为`false`，意味着他的边永远从左部指向右部。

二分图分成左右两部`left`，`right`。

可以通过以下函数去设置：

```cpp
void set_left(int left); // right = node_count - left
void set_right(int right); // left =  node_count - right
void set_left_right(int left, int right); // node_count = left +
```

可以通过以下函数去获取：

```cpp
int& left();
int& right();

int cleft() const;
int cright() const;
```

如果不设置，左部大小是 $-1$ ，在生成的时候随机左右部大小。

二分图有额外两个参数：

1. `different_part`

    表示的含义是左部和右部的编号是否用不同的集合。

    如果为`true`，那么左部编号为从  $begin\textunderscore node$ 开始 $left$ 个数字，右部编号为从  $begin\textunderscore node$ 开始 $right$ 个数字。

    如果为`false`，那么左部和右部的编号一起是从  $begin\textunderscore node$ 开始 $node$ 个数字。

    默认值是`false`。

    可以通过`void set_different_part(bool different_part)`设置。

2. `output_node_type`

    表示的含义是点数输出时候的形式。

    内部有一个枚举类：

    ```cpp
    enum NodeOutputFormat {
        Node,       //输出格式："点数"
        LeftRight,  //输出格式："左部大小 右部大小"
        NodeLeft,   //输出格式："点数 左部大小"
        NodeRight   //输出格式："点数 右部大小"
    };
    ```
    默认值是`Node`。

    可以通过以下函数修改：

    ```cpp
    void set_node_output_format(NodeOutputFormat format);
    void use_format_node();
    void use_format_left_right();
    void use_format_node_left();
    void use_format_node_right();
    ```
    

[示例代码1](../../../examples/bipartite_graph1.cpp)：
生成一个 $5$ 个结点， $6$ 条边的二分图。

[示例代码2](../../../examples/bipartite_graph2.cpp)：
生成一个左部大小为 $3$ ， 右部大小为 $4$ ， $10$ 条边的二分图。
保证它一定连通，它的边不一定是从左部指向右部的形式。

[示例代码3](../../../examples/bipartite_graph3.cpp)：
生成一个 $7$ 个结点， $10$ 条边, 结点起始编号为 $0$ ，左部大小为 $3$ 的二分图。
它左部的编号为 $0\sim2$ ,右部的编号为 $0\sim3$ 。
输出的形式是`LeftRight`。

[返回上级](./summary.md)

[返回目录](../../home.md)
