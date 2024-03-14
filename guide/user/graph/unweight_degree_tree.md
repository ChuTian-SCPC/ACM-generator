## unweight::DegreeTree

生成一棵树，树上每个结点的度数不会超过限制。

构造函数是：
```cpp
DegreeTree(
    int node = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_degree = -1
);
```

可以通过函数`void set_max_degree(int max_degree)`来设置度数的限制。

如果不设置，`max_degree`的默认值为 $-1$ ，度数的限制将随机生成。

除此之外，也可以在随机生成的时候指定度数的限制：

1. `void gen(int max_degree);`

    生成度数限制为max_degree的树。

2. `void gen(T l_limit, U r_limit);`

    生成度数限制在[l_limit, r_limit]之间的树。

3. `void gen(const char *format, ...);`

    生成度数限制在指定范围内的函数。
    指定范围的格式详细看[rand_int](../rand/rand_int.md)中的格式。

[示例代码](./unweight_degree_tree.md)

生成一棵 $10$ 个结点的无根树，每个结点的度数不超过 $3$ 。

[返回上级](./summary.md)

[返回目录](../../home.md)