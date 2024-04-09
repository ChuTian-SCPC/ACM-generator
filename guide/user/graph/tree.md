## 树(Tree)

生成一棵普通的树。

构造函数是：
```cpp
// 无权
Tree(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    TreeGenerator tree_generator = RandomFather
);

// 带点权
Tree<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    TreeGenerator tree_generator = RandomFather  
);

// 带边权
Tree<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    EdgeGenFunction edges_weight_function = nullptr,
    TreeGenerator tree_generator = RandomFather  
);

// 带点权和边权
Tree<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr,
    TreeGenerator tree_generator = RandomFather  
);
```



`TreeGenerator`是一个枚举类，它有两种生成树的方式：

```cpp
enum TreeGenerator {
    RandomFather,
    Pruefer
};
```


可以通过以下函数指定生成树的方式：

```cpp
void set_tree_generator(TreeGenerator tree_generator);

void use_random_father();
void use_pruefer();
```

生成的期望树高分别是 $O(\log n)$ 和 $O(\sqrt{n})$  ，默认使用的是第一种。可以参考[OI Wiki](https://oi-wiki.org/contest/problemsetting/#%E7%94%9F%E6%88%90%E9%9A%8F%E6%9C%BA%E6%A0%91)相关的介绍。

`TreeGenerator`在每个命名空间（`unweight`，`node_weight`，`edge_weight`，`both_weight`）内都有。



[示例代码1](../../../examples/tree1.cpp)：
生成一棵 $5$ 个结点的有根树，结点起始编号为 $1$ ，根为 $2$ 。包含四种类型。

[示例代码2](../../../examples/tree2.cpp)：
生成一棵 $5$ 个结点的有根树，结点起始编号为 $0$ ，根为 $4$ 。
它的边不一定是`father son`的形式。
输出的时候不输出它的根。

[示例代码3](../../../examples/tree3.cpp)：
生成一棵 $5$ 个结点的无根树，结点起始编号为 $0$ 。
生成的方法是随机一个Pruefer序列还原。

[返回上级](./summary.md)

[返回目录](../../home.md)