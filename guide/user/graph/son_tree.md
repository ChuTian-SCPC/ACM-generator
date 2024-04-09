## 限制最大儿子数的树(SonTree)

生成一棵有根树，树上每个结点的儿子数不会超过限制。

构造函数是：
```cpp
// 无权
SonTree(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_son = -1
);

// 带点权
SonTree<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_son = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
SonTree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_son = -1,
    EdgeGenFunction edges_weight_function = nullptr
);

// 带点权和边权
SonTree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_son = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

**注意** ： 这种树必须为有根树。

因为是有根树，所以以下函数被禁用：

```cpp
void set_is_rooted(bool is_rooted);
```

树的最大儿子数是`max_son`。

可以通过以下函数去设置：

```cpp
void set_max_son(int max_son);
```

可以通过以下函数去获取：

```cpp
int& max_son();
int cmax_son() const;
```

如果不设置，`max_son`的默认值为 $-1$ ，树的最大儿子数采取随机值。

[示例代码](../../../examples/son_tree.cpp)

生成一棵 $10$ 个结点的有根树，每个结点的儿子数不超过 $2$ ,即一棵二叉树。

[返回上级](./summary.md)

[返回目录](../../home.md)