## 菊花带链(FlowerChain)

生成一个菊花带链。

构造函数是：
```cpp
// 无权
FlowerChain(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    int flower_size = -1
);

// 带点权
FlowerChain<NodeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    int flower_size = -1,
    NodeGenFunction nodes_weight_function = nullptr
);

// 带边权
FlowerChain<EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    int flower_size = -1,
    EdgeGenFunction edges_weight_function = nullptr
);


// 带点权和边权
FlowerChain<NodeType, EdgeType>(
    int node_count = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1,
    int flower_size = -1,
    NodeGenFunction nodes_weight_function = nullptr,
    EdgeGenFunction edges_weight_function = nullptr
);
```

菊花带链分成菊花和链，两个的大小分别是`flower_size`和`chain_size`。

可以通过以下函数去设置：

```cpp
void set_flower_size(int flower_size);                       // chain_size = node_count - flower_size
void set_chain_size(int chain_size);                         // flower_size =  node_count - chain_size
void set_flower_chain_size(int flower_size, int chain_size); // node_count = flower_size + chain_size
```

可以通过以下函数去获取：

```cpp
int flower_size() const;
int chain_size() const;

int flower_size_ref();
int chain_size_ref();
```

如果不设置，`flower_size`大小是 $-1$ ，在生成的时候随机菊花和链的大小。

[示例代码](../../../examples/flower_chain.cpp)：
生成一棵 $10$ 个结点的菊花带链，菊花大小为 $6$ , 链大小为 $4$ 。

[返回上级](./summary.md)

[返回目录](../../home.md)