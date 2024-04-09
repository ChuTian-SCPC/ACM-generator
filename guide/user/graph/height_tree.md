## 指定高度的树(HeightTree)

生成一个指定高度的树。

构造函数是：
```cpp
// 无权
HeightTree(
    int node_count = 1, 
    int begin_node = 1, 
    int root = 1, 
    int height = -1
);

// 带点权
HeightTree<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    int root = 1, 
    int height = -1,
    NodeGenFunction nodes_weight_function = nullptr 
);

// 带边权
HeightTree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    int root = 1, 
    int height = -1,
    EdgeGenFunction edges_weight_function = nullptr
);

// 带点权和边权
HeightTree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    int root = 1, 
    int height = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```
**注意** ： 这种树必须为有根树，树的高度以根节点为 $1$ 开始计算。

因为是有根树，所以以下函数被禁用：

```cpp
void set_is_rooted(bool is_rooted);
```

树的高度是`height`。

可以通过以下函数去设置：

```cpp
void set_height(int height);
```

可以通过以下函数去获取：

```cpp
int& height();
int cheight() const;
```

如果不设置，`height`的默认值为 $-1$ ，树的高度采取随机一个树高。



[示例代码](../../../examples/height_tree.cpp)

生成一棵 $5$ 个结点的有根树，结点起始编号为 $1$ ，根为 $2$ ， 高度为 $3$ 。



[返回上级](./summary.md)

[返回目录](../../home.md)