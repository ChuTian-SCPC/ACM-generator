## unweight::Cactus

生成一个仙人掌。

构造函数是：
```cpp
Cactus(
    int node = 1, 
    int side = 0, 
    int begin_node = 1
);
```

仙人掌是无重边，无自环，一定连通的无向图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
void set_multiply_edge(bool multiply_edge);
void set_self_loop(bool self_loop);
void set_connect(bool connect);
```

[示例代码](../../../examples/unweight_cactus.cpp)：
生成一棵 $9$ 个结点, 边数为 $12$ 的仙人掌。

[返回上级](./summary.md)

[返回目录](../../home.md)